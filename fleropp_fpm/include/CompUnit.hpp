#ifndef COMP_UNIT_HPP
#define COMP_UNIT_HPP

#include "ISOLoader.hpp"
#include "CompilerDefaults.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/process/child.hpp>
#include <boost/process/search_path.hpp>

#include <dlfcn.h>
#include <unistd.h>

// fleropp_fpm project namespace
namespace fleropp_fpm {
    template <class T> 
    class CompUnit : public ISOLoader<T> { 
      public:
        /**
         * Constructor.
         * 
         * \param[in] basename The basename for all files related to the shared
         *                     object.
         * \param[in] lib_dir The directory in which all files related to the
         *                    shared object are located (default "/var/www/html/").
         * \param[in] src_ext The extension used by source files (default ".cpp").  
         * \param[in] lib_ext The extension used by shared library files (default ".so").  
         * \param[in] alloc_sym The symbol name used for an allocator function with
         *                      C linkage (default "allocator").
         * \param[in] del_sym   The symbol name used for a delete function with
         *                      C linkage (default "deleter").  
         */
        CompUnit(const std::string &shared_object,
                    const std::vector<std::string> &src_path_list,
                    const std::string &compiler = "g++",
                    const std::vector<std::string> &args = {},
                    const std::string &alloc_sym = "allocator", 
                    const std::string &del_sym = "deleter") :
                        _handle{nullptr}, _shared_object{shared_object},
                        _src_path_list{src_path_list},
                        _alloc_sym{alloc_sym}, _del_sym{del_sym},
                        m_compiler{compiler}, _args{args},
                        _open{false} {

            if (compiler_defaults::compiler_map.find(compiler) == compiler_defaults::compiler_map.end() && args.empty()) {
                // Warning/Error that compiler was not found to have default argument list, will revert to default parameters.
                m_compiler = "g++";
                _args = compiler_defaults::compiler_map.at(m_compiler);
            }
            else if (args.empty()){
                _args = compiler_defaults::compiler_map.at(m_compiler);
            }
            _args.emplace_back(shared_object);
            _args.insert(std::end(_args), std::begin(_src_path_list), std::end(_src_path_list));
        }

        void open_lib() override {
            // Only do something if the library is not currently open
            if (!_open) {
                // RTLD_NOW, we could try RTLD_LAZY for performance
                if (!(_handle = ::dlopen(_shared_object.c_str(), RTLD_NOW | RTLD_GLOBAL))) {
                    std::cerr << ::dlerror() << '\n';
                } else {
                    _open = true;
                }
            }
        }

        void close_lib() override {
            // Only do something if the library is currently open.
            if (_open) {
                if (::dlclose(_handle) != 0) {
                    std::cerr << ::dlerror() << '\n';
                } else {
                    _open = false;
                }
            }
        }

        std::shared_ptr<T> get_instance() override {
            // If the library was recompiled, close it so we can reopen
            if (recompile()) {
                close_lib();
            }
            open_lib();

            // Type alias for a function pointer to a function that returns a
            // pointer to T
            using alloc_fun_t = T *(*)();
            // Type alias for a function pointer to a void function that 
            // accepts a pointer to T
            using del_fun_t = void (*)(T *);

            // dlsym returns a void pointer, so we reinterpret cast it to our
            // type aliases above
            auto alloc_fun = reinterpret_cast<alloc_fun_t>(::dlsym(_handle, _alloc_sym.c_str()));
            auto del_fun = reinterpret_cast<del_fun_t>(::dlsym(_handle, _del_sym.c_str()));

            // If either is nullptr, something went wrong
            if (!alloc_fun || !del_fun) {
                close_lib();
                std::cerr << ::dlerror() << '\n';
            }

            // Return a shared_ptr to T (whose raw pointer is acquired by 
            // calling alloc_fun) with a custom deleter closure that calls
            // del_fun.
            return std::shared_ptr<T>(alloc_fun(), [del_fun] (T *p) { del_fun(p); });
        } 

      private:
        void *_handle;
        std::string _shared_object;
        std::vector<std::string> _src_path_list;
        std::string _alloc_sym;
        std::string _del_sym;
        std::string m_compiler;
        std::vector<std::string> _args;
        bool _open;

        // Checks if the source file was modified
        bool was_modified() const {
            // If library file does not exist, we consider the source file to
            // have been modified. Otherwise, we return whether the source
            // is newer than the library based on mtime.
            if (std::filesystem::exists(_shared_object)) {
                const auto so_mtim = std::filesystem::last_write_time(_shared_object);
                return std::any_of(std::begin(_src_path_list), std::end(_src_path_list), 
                                    [so_mtim] (auto src) { 
                                        return std::filesystem::last_write_time(src) > so_mtim; 
                                    });
            } 
            return true;
        } 

        // Recompile if necessary
        bool recompile() const {
            namespace bp = boost::process;
            if (was_modified()) {
                bp::child chld(bp::search_path(m_compiler), _args);
                chld.wait();
                return true;
            }            
            return false;
        }
    };
}

#endif /* COMP_UNIT_HPP */