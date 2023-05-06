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

#include "CompilerDefaults.hpp"
#include "ISOLoader.hpp"
#include "logging.hpp"

#include <atomic>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/process.hpp>
#include <boost/process/child.hpp>
#include <boost/process/search_path.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "spdlog/spdlog.h"

#include <dlfcn.h>
#include <unistd.h>

/**
 * \namespace fleropp::fpm 
 */
namespace fleropp::fpm {
    using error_fun_t = std::function<std::string(std::string, std::string, std::string)>;
    /**
     * \class CompUnit
     * \brief Class template that represents a dynamically-loaded shared object
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
         * \brief CompUnit constructor
         * 
         * \param[in] shared_object The name of the shared object that represents
         *                          this compilation unit
         * \param[in] src_path_list A vector containing a list of paths to all of
         *                          the source files belonging to this compilation
         *                          unit
         * \param[in] error_generator A function that produces the source code
         *                            to be compiled when an error is encountered
         *                            while compiling the actual sources. The function
         *                            shall accept three parameters, ideally inserting
         *                            them into the source code it produces:
         *                              1. The contained class name as a string
         *                              2. The compiler stderr stream contents as a string
         *                              3. The shared object file name as a string
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
                    error_fun_t error_generator, 
                    const std::string &compiler = "g++",
                    const std::vector<std::string> &args = {},
                    const std::string &alloc_sym = "allocator", 
                    const std::string &del_sym = "deleter") :
                        m_handle{nullptr}, m_shared_object{shared_object},
                        m_src_path_list{src_path_list}, m_error_generator{error_generator},
                        m_alloc_sym{alloc_sym}, m_del_sym{del_sym},
                        m_compiler{compiler}, m_args{args} {

            if (compiler_defaults::compiler_map.find(compiler) == compiler_defaults::compiler_map.end() && args.empty()) {
                // Warning/Error that compiler was not found to have default argument list, will revert to default parameters.
                spdlog::get("compiler")->warn("{} was not found to have default argument list, will revert to g++ default parameters", m_compiler);
                m_compiler = "g++";
                m_args = compiler_defaults::compiler_map.at(m_compiler);
            } else if (args.empty()) {
                spdlog::debug("{} has no arguments, will use default parameters", m_compiler);
                m_args = compiler_defaults::compiler_map.at(m_compiler);
            }

            // Handle the `@OBJECT` metavariable, replacing it with the shared object name
            std::replace(std::begin(m_args), std::end(m_args), std::string{"@OBJECT"}, shared_object);

            // Handle the `@SOURCES` metavariable, replacing it with the list of source files
            auto sources_it = std::find(std::begin(m_args), std::end(m_args), "@SOURCES");
            if (sources_it != std::end(m_args)) {
                sources_it = m_args.erase(sources_it);
                m_args.insert(sources_it, std::begin(m_src_path_list), std::end(m_src_path_list));
            }
            spdlog::info("Compiler loaded: '{}'. Args loaded: '{}'", m_compiler, fmt::join(m_args, ", "));
        }

        /**
         * \brief CompUnit constructor
         * 
         * This constructor is used when a CompUnit object is used
         * only to control the dynamic loading of a DSO without
         * accompanying code.
         * 
         * \param [in] shared_object The name of the shared object that represents
         *                          this compilation unit
         * \param [in] alloc_sym The allocator symbol
         * \param [in] del_sym The deleter symbol
         * 
        */
       CompUnit(const std::string& shared_object,
                const std::string& alloc_sym = "allocator",
                const std::string& del_sym = "deleter"):
            m_handle{nullptr}, m_shared_object{shared_object},
            m_src_path_list{}, m_error_generator{}, m_alloc_sym{alloc_sym},
            m_del_sym{del_sym}, m_compiler{}, m_args{} {}

        /**
         * \brief Function that loads shared objects into memory
         * 
         * Loads the shared object into memory, if it is not already open. As
         * with any call to `::dlopen`, the reference count to the shared object
         * will be incremented. 
         */
        void open_lib() final {
            // Only do something if the library is not currently open
            static std::shared_mutex open_mtx;
            if (!m_open.test()) {
                {   
                    // We attempt to acquire an exclusive lock, but don't block if we
                    // cannot acquire it. 
                    std::unique_lock lock(open_mtx, std::try_to_lock);
                    if (lock && !m_open.test()) {
                        if ((std::filesystem::file_size(m_shared_object) == 0) || 
                            !(m_handle = ::dlopen(m_shared_object.c_str(), RTLD_LAZY | RTLD_LOCAL))) {
                            spdlog::error("Unable to load DSO '{}': {}", m_shared_object, ::dlerror());
                        } else {
                            m_open.test_and_set();
                            spdlog::debug("Opened '{}'", m_shared_object);
                        }
                    }
                } 
            }
            {
                std::shared_lock lock(open_mtx);
            }
        }

        /**
         * \brief Function that unloads shared objects from memory
         * 
         * Unloads the shared object from memory, if: it is already open AND
         * decrementing the reference count would lead to a reference count
         * of zero AND no other translation units depend on its exported
         * symbols. The OS does not guarantee if or when the DSO will be
         * unloaded, but under normal circumstances it should be unloaded
         * upon invocation of this function.
         */
        void close_lib() final {
            // Only do something if the library is currently open.
            if (m_open.test()) {
                // If the handle is valid (i.e., not `nullptr`) *but* we fail to close
                // it, log an error. Otherwise, we assume the handle was never acquired
                // in the first place (or freed at some point earlier) and clear the flag.
                // 
                // The first case also prevents invocation of `::dlclose(nullptr)`, a scenario
                // that, although not well-documented, is likely to be bad news.
                if (m_handle && (::dlclose(m_handle) != 0)) {
                    spdlog::error("Unable to unload DSO '{}': {}", m_shared_object, ::dlerror());
                } else {
                    m_open.clear();
                    spdlog::debug("Closed '{}'", m_shared_object);
                }
            }
        }

        /**
         * \brief Function that recompiles (if necessary) and returns a pointer to the relevant class
         * 
         * Provides a smart pointer to an instance of the class contained
         * within this compilation unit. The compilation unit will be
         * compiled prior to returning an instance if the shared object
         * on disk is stale OR not yet present. If present, the shared object
         * residing in memory will be reloaded prior to return if compilation 
         * occurs.
         * 
         * \return A `std::shared_ptr` to an instance of the contained class.
         */
        template<typename... Args>
        std::shared_ptr<T> get_instance(Args&& ...args) {
            // Type alias for a function pointer to a function that returns a
            // pointer to `T`
            using alloc_fun_t = T *(*)(Args&& ...);
            // Type alias for a function pointer to a void function that 
            // accepts a pointer to `T`
            using del_fun_t = void (*)(T *);

            // We only need to do the recompile when there is source code
            // available
            if (!m_src_path_list.empty()) {
                // If the library was recompiled, close it so we can reopen
                recompile();
            }

            open_lib();

            // dlsym returns a void pointer, so we reinterpret cast it to our
            // type aliases above
            auto alloc_fun = reinterpret_cast<alloc_fun_t>(::dlsym(m_handle, m_alloc_sym.c_str()));
            auto del_fun = reinterpret_cast<del_fun_t>(::dlsym(m_handle, m_del_sym.c_str()));

            // If either is nullptr, something went wrong
            if (!alloc_fun || !del_fun) {
                close_lib();
                spdlog::error("Unable to resolve allocator and/or deleter symbol in '{}': {}", m_shared_object, ::dlerror());
                // Return a default-constructed std::shared_ptr whose value shall be `nullptr`
                return {};
            }

            // Return a shared_ptr to T (whose raw pointer is acquired by 
            // calling alloc_fun) with a custom deleter closure that calls
            // del_fun.
            spdlog::debug("Returning instance of '{}'", m_shared_object);
            return std::shared_ptr<T>{alloc_fun(std::forward<Args>(args)...), [del_fun] (T *p) { del_fun(p); }};
        }

      private:
        void* m_handle;
        std::string m_shared_object;
        std::vector<std::string> m_src_path_list;
        error_fun_t m_error_generator;
        std::string m_alloc_sym;
        std::string m_del_sym;
        std::string m_compiler;
        std::vector<std::string> m_args;
        static inline std::atomic_flag m_open = ATOMIC_FLAG_INIT;

        // Checks if the source file was modified
        bool was_modified() const {
            // If library file does not exist, we consider the source file to
            // have been modified. Otherwise, we return whether the source
            // is newer than the library based on mtime.
            if (std::filesystem::exists(m_shared_object)) {
                const auto so_mtim = std::filesystem::last_write_time(m_shared_object);
                //spdlog::debug("'{}' last modified on {}", m_shared_object, so_mtim);
                return !m_src_path_list.empty() &&
                       std::any_of(std::begin(m_src_path_list), std::end(m_src_path_list), 
                                   [so_mtim] (auto src) { 
                                        return std::filesystem::last_write_time(src) > so_mtim; 
                                    });
            } 
            return true;
        }

        // Compiles the shared object and provides access to the compiler's
        // exit code and the contents of the stderr stream.
        std::pair<int, std::string> compile(const std::string& compiler, 
                                            const std::vector<std::string>& args) const {
            namespace bp = boost::process;
            // Set up and execute the compiler
            // We use boost::process to simplify this process
            //
            // Important note: we capture the stderr stream from the compiler
            // so that it can be logged in the event of a compilation failure.
            boost::asio::io_context ioc;
            std::future<std::string> stderr;
            bp::child chld(bp::search_path(compiler), args, bp::std_in.close(),
                           bp::std_out > bp::null, bp::std_err > stderr, ioc);
            ioc.run();
            chld.wait();
            return {chld.exit_code(), stderr.get()};
        }

        // 0-ary overload of `compile` for the most common use case
        std::pair<int, std::string> compile() const { return compile(m_compiler, m_args); } 

        // Using source code from the error-generation function provided in the constructor, 
        // compile a dummy shared object that will provide error information in a contextually
        // relevant manner.
        void compile_error_object(const std::string& error_text) const {
            // Generate and stringify a UUID
            auto tmp_name = "_" + boost::uuids::to_string(boost::uuids::random_generator()());

            // Convert the UUID to a valid C++ identifier
            tmp_name.erase(std::remove(std::begin(tmp_name), std::end(tmp_name), '-'), std::end(tmp_name));

            // Create a source file name from the UUID and create it within a random tmpdir
            const auto tmp_file = tmp_name + ".cpp";
            const auto tmp_dir = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
            const auto tmp_path = tmp_dir / tmp_file;
            boost::filesystem::create_directories(tmp_dir);
            std::ofstream fd{tmp_path};
            
            // Write the desired source code to the temporary file, ready for compilation 
            fd << m_error_generator(tmp_name, error_text, m_shared_object);
            
            spdlog::info("Compiling dummy error page for '{}'", m_shared_object);
            const auto [e_exit_code, e_stderr] = compile(m_compiler, {"-std=c++20", "-shared", "-fPIC", "--no-gnu-unique",
                                                                      "-o", m_shared_object, tmp_path.string()});

            // Remove the temporary directory and its contents, as it is no longer needed after compilation
            boost::filesystem::remove_all(tmp_dir);

            // Last resort; if the supplied error source generation function cannot be compiled,
            // we fall back to creating an empty dummy file or setting the mtime on the existing file. 
            if (e_exit_code) {
                spdlog::get("compiler")->warn("Non-zero exit of {} ($? -> {}):\n{}", m_compiler, e_exit_code, e_stderr);
                // Update the modification time of the existing shared object, if it exists.
                // If it does not exist, we create an empty file to serve as a 
                // placeholder.
                //
                // This signals to `CompUnit` that a recompile has happened---and it
                // shouldn't happen again until another modification. A failed compile
                // is, in all likelihood, not going to be resolved until another
                // modification occurs.
                if (std::filesystem::exists(m_shared_object)) {
                    // Set the mtime to now
                    const auto new_mtime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    boost::filesystem::last_write_time(m_shared_object, new_mtime);
                    spdlog::debug("Updated dummy error page for '{}' modification time to '{}' after failed compile", m_shared_object, new_mtime);
                } else {
                    // Create an empty file
                    std::ofstream dummy{m_shared_object};
                    spdlog::debug("Created empty placeholder for '{}' after failed compile of dummy error page", m_shared_object);
                }
            } else {
                spdlog::info("Successfully compiled dummy error page for '{}'", m_shared_object);
            }
        }

        // Recompile if necessary
        void recompile() {
            static std::shared_mutex recompile_mtx;
            if (was_modified()) {
                {
                    // We attempt to acquire an exclusive lock, but don't block if we
                    // cannot acquire it.
                    std::unique_lock lock(recompile_mtx, std::try_to_lock);
                    if (lock && was_modified()) {
                        spdlog::info("Compiling '{}'", m_shared_object);
                        const auto [exit_code, stderr] = compile();

                        // If the compilation failed, log the exit code and the errors that were
                        // reported. If it was successful, we can simply close the library.
                        if (exit_code) {
                            spdlog::get("compiler")->warn("Non-zero exit of {} ($? -> {}):\n{}", m_compiler, exit_code, stderr);
                            compile_error_object(stderr); 
                        } else {  // exit_code == 0 so the page was sucessfully recompiled with no issues.
                            spdlog::info("Sucessfully compiled '{}'", m_shared_object);
                        }
                        close_lib();
                    }
                }
            }
            {
                std::shared_lock lock(recompile_mtx);   
            }
        }
    };
}

#endif /* COMP_UNIT_HPP */
