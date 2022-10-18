#ifndef COMP_UNIT_HPP
#define COMP_UNIT_HPP

#include "ISOLoader.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include <dlfcn.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
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
                    std::vector<std::string> &src_path_list,
                    const std::string &alloc_sym = "allocator", 
                    const std::string &del_sym = "deleter") :
                        _handle{nullptr}, _shared_object{shared_object},
                        _src_path_list{std::move(src_path_list)},
                        _alloc_sym{alloc_sym}, _del_sym{del_sym} {}

        void open_lib() override {
            // Only do something if the library is not currently open
            if (!_open) {
                // RTLD_NOW, we could try RTLD_LAZY for performance
                if (!(_handle = ::dlopen(_shared_object.c_str(), RTLD_NOW))) {
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

        bool _open = false;

        // Checks if the source file was modified
        bool was_modified() const {
            // Source and library file stat structs
            //struct stat src_stat, lib_stat;
            //const auto src_ret = ::stat(_src_path.c_str(), &src_stat) == 0;
            //const auto lib_ret = ::stat(_lib_path.c_str(), &lib_stat) == 0;
            
            // If library file does not exist, we consider the source file to
            // have been modified. Otherwise, we return whether the source
            // is newer than the library based on mtime.
            if (std::filesystem::exists(_shared_object)) {
                const auto src_mtim = std::filesystem::last_write_time(_src_path_list[0]);
                const auto so_mtim = std::filesystem::last_write_time(_shared_object);
                return src_mtim > so_mtim;
            } 

            return true;
        } 

        // Recompile if necessary
        bool recompile() {
            if (was_modified()) {
                // Fork and exec
                auto child_pid = ::fork();
                if (!child_pid) {
                    ::chdir(_shared_object.substr(0,_shared_object.find_last_of("/")).c_str());
                    ::execlp("g++", "g++", _src_path_list[0].c_str(), "-std=c++17",
                                "-shared", "-fPIC", "-o", _shared_object.c_str(),
                                nullptr);
                }
                ::waitpid(child_pid, nullptr, 0);
                return true;
            }            
            return false;
        }
    };
}

#endif /* SO_LOADER_HPP */