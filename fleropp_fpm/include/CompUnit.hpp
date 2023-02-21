#ifndef COMP_UNIT_HPP
#define COMP_UNIT_HPP

/**
 * \file CompUnit.hpp
 * \brief Flero++ compilation unit.
 * 
 * This file contains the header-only implementation of compilation units that
 * are tracked and updated as needed by each process spawned by the process 
 * manager.
 */

#include "ISOLoader.hpp"
#include "CompilerDefaults.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/process.hpp>
#include <boost/process/child.hpp>
#include <boost/process/search_path.hpp>

#include "spdlog/spdlog.h"

#include <dlfcn.h>
#include <unistd.h>

/**
 * \namespace fleropp::fpm 
 */
namespace fleropp::fpm {
    /**
     * \brief Class template representinig a dynamically-loaded shared object.
     * 
     * A compilation unit is responsible for:
     * - Tracking the state of its shared object in memory.
     * - Providing an instance of the underlying class that serves as an interface
     *   with the shared library in memory.
     * - Recompiling itself in the event that the in-memory version becomes stale.
     * 
     * \tparam T The class that is exposed via invocation of `CompUnit<T>::get_instance`. 
     */
    template <class T> 
    class CompUnit : public ISOLoader<T> { 
      public:
        /**
         * Constructor.
         * 
         * \param[in] shared_object The name of the shared object that represents
         *                          this compilation unit.
         * \param[in] src_path_list A vector containing a list of paths to all of
         *                          the source files belonging to this compilation
         *                          unit.
         * \param[in] compiler The name of the compiler executable to be used to
         *                     compile this compilation unit. Will search $PATH.
         *                     (default "g++").
         * \param[in] args A vector of additional arguments supplied to the compiler.
         * \param[in] alloc_sym The symbol name used for an allocator function with
         *                      C linkage (default "allocator"). Used on invocation
         *                      of `CompUnit<T>::get_instance`.
         * \param[in] del_sym The symbol name used for a delete function with
         *                    C linkage (default "deleter"). Used on invocation of
         *                    `CompUnit<T>::get_instance`.
         */
        CompUnit(const std::string &shared_object,
                    const std::vector<std::string> &src_path_list,
                    const std::string &compiler = "g++",
                    const std::vector<std::string> &args = {},
                    const std::string &alloc_sym = "allocator", 
                    const std::string &del_sym = "deleter") :
                        m_handle{nullptr}, m_shared_object{shared_object},
                        m_src_path_list{src_path_list},
                        m_alloc_sym{alloc_sym}, m_del_sym{del_sym},
                        m_compiler{compiler}, m_args{args},
                        m_open{false} {

            if (compiler_defaults::compiler_map.find(compiler) == compiler_defaults::compiler_map.end() && args.empty()) {
                // Warning/Error that compiler was not found to have default argument list, will revert to default parameters.
                spdlog::get("compiler")->warn("{} was not found to have default argument list, will revert to g++ default parameters", m_compiler);
                m_compiler = "g++";
                m_args = compiler_defaults::compiler_map.at(m_compiler);
            } else if (args.empty()) {
                spdlog::debug("{} has no arguments, will use default parameters", m_compiler);
                m_args = compiler_defaults::compiler_map.at(m_compiler);
            }
            m_args.emplace_back(shared_object);
            m_args.insert(std::end(m_args), std::begin(m_src_path_list), std::end(m_src_path_list));
            spdlog::info("Compiler loaded: '{}'. Args loaded: '{}'", m_compiler, fmt::join(m_args, ", "));
        }

        /**
         * Loads the shared object into memory, if it is not already open. As
         * with any call to `::dlopen`, the reference count to the shared object
         * will be incremented. 
         */
        void open_lib() final {
            // Only do something if the library is not currently open
            if (!m_open) {
                static std::shared_mutex open_mtx;
                {   
                    // We attempt to acquire an exclusive lock, but don't block if we
                    // cannot acquire it. 
                    std::unique_lock lock(open_mtx, std::try_to_lock);
                    if (lock) {
                        if (!(m_handle = ::dlopen(m_shared_object.c_str(), RTLD_LAZY | RTLD_LOCAL))) {
                            spdlog::error("Unable to load DSO '{}': {}", m_shared_object, ::dlerror());
                        } else {
                            m_open = true;
                            spdlog::debug("Opened '{}'", m_shared_object);
                        }
                    }
                }
                // Having skipped the previous section if we couldn't acquire
                // a lock, we now block until the thread that could acquire a
                // lock finishes. All threads should be able to acquire this
                // lock once the exclusive lock is released. 
                {
                    std::shared_lock lock(open_mtx);
                }
            }
        }

        /**
         * Unloads the shared object from memory, if: it is already open AND
         * decrementing the reference count would lead to a reference count
         * of zero AND no other translation units depend on its exported
         * symbols. The OS does not guarantee if or when the DSO will be
         * unloaded, but under normal circumstances it should be unloaded
         * upon invocation of this function.
         */
        void close_lib() final {
            // Only do something if the library is currently open.
            if (m_open) {
                if (::dlclose(m_handle) != 0) {
                    spdlog::error("Unable to unload DSO '{}': {}", m_shared_object, ::dlerror());
                } else {
                    m_open = false;
                    spdlog::debug("Closed '{}'", m_shared_object);
                }
            }
        }

        /**
         * Provides a smart pointer to an instance of the class contained
         * within this compilation unit. The compilation unit will be
         * compiled prior to returning an instance if the shared object
         * on disk is stale OR not yet present. If present, the shared object
         * residing in memory will be reloaded prior to return if compilation 
         * occurs.
         * 
         * \return A `std::shared_ptr` to an instance of the contained class.
         */
        std::shared_ptr<T> get_instance() final {
            // Type alias for a function pointer to a function that returns a
            // pointer to T
            using alloc_fun_t = T *(*)();
            // Type alias for a function pointer to a void function that 
            // accepts a pointer to T
            using del_fun_t = void (*)(T *);

            // If the library was recompiled, close it so we can reopen
            recompile();
            open_lib();

            // dlsym returns a void pointer, so we reinterpret cast it to our
            // type aliases above
            auto alloc_fun = reinterpret_cast<alloc_fun_t>(::dlsym(m_handle, m_alloc_sym.c_str()));
            auto del_fun = reinterpret_cast<del_fun_t>(::dlsym(m_handle, m_del_sym.c_str()));

            // If either is nullptr, something went wrong
            if (!alloc_fun || !del_fun) {
                close_lib();
                spdlog::error("Unable to resolve allocator and/or deleter symbol in '{}': {}", m_shared_object, ::dlerror());
                return std::shared_ptr<T>{};
            }

            // Return a shared_ptr to T (whose raw pointer is acquired by 
            // calling alloc_fun) with a custom deleter closure that calls
            // del_fun.
            return std::shared_ptr<T>{alloc_fun(), [del_fun] (T *p) { del_fun(p); }};
        } 

      private:
        void *m_handle;
        std::string m_shared_object;
        std::vector<std::string> m_src_path_list;
        std::string m_alloc_sym;
        std::string m_del_sym;
        std::string m_compiler;
        std::vector<std::string> m_args;
        bool m_open;

        // Checks if the source file was modified
        bool was_modified() const {
            // If library file does not exist, we consider the source file to
            // have been modified. Otherwise, we return whether the source
            // is newer than the library based on mtime.
            if (std::filesystem::exists(m_shared_object)) {
                const auto so_mtim = std::filesystem::last_write_time(m_shared_object);
                //spdlog::debug("'{}' last modified on {}", m_shared_object, so_mtim);
                return std::any_of(std::begin(m_src_path_list), std::end(m_src_path_list), 
                                    [so_mtim] (auto src) { 
                                        return std::filesystem::last_write_time(src) > so_mtim; 
                                    });
            } 
            return true;
        } 

        // Recompile if necessary
        void recompile() {
            namespace bp = boost::process;
            if (was_modified()) {
                static std::shared_mutex recompile_mtx;
                {
                    // We attempt to acquire an exclusive lock, but don't block if we
                    // cannot acquire it.
                    std::unique_lock lock(recompile_mtx, std::try_to_lock);
                    if (lock) {
                        spdlog::info("Compiling '{}'", m_shared_object);
                        boost::asio::io_context ioc;
                        std::future<std::string> stderr;
                        bp::child chld(bp::search_path(m_compiler), m_args, bp::std_in.close(),
                                    bp::std_out > bp::null, bp::std_err > stderr, ioc);
                        ioc.run();
                        chld.wait();
                        const auto exit_code = chld.exit_code();
                        if (exit_code) {
                            spdlog::get("compiler")->warn("Non-zero exit of {} ($? -> {}):\n{}", m_compiler, exit_code, stderr.get());
                        } else { // exit_code == 0 so the page was sucessfully recompiled with no issues.
                            spdlog::info("Sucessfully compiled '{}'", m_shared_object);
                            close_lib();
                        }
                    }
                }
                // Having skipped the previous section if we couldn't acquire
                // a lock, we now block until the thread that could acquire a
                // lock finishes. All threads should be able to acquire this
                // lock once the exclusive lock releases.
                {
                    std::shared_lock lock(recompile_mtx);
                }
            }            
        }
    };
}

#endif /* COMP_UNIT_HPP */
