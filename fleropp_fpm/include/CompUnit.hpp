#ifndef COMP_UNIT_HPP
#define COMP_UNIT_HPP

#include "ISOLoader.hpp"

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
        CompUnit(const std::string &basename,
                    const std::string &lib_dir = "/var/www/html/", 
                    const std::string &src_ext = ".cpp",
                    const std::string &lib_ext = ".so",
                    const std::string &alloc_sym = "allocator", 
                    const std::string &del_sym = "deleter") :
                        _handle{nullptr}, _basename{basename},
                        _lib_dir{lib_dir}, _src_ext{src_ext},
                        _lib_ext{lib_ext}, _alloc_sym{alloc_sym},
                        _del_sym{del_sym} {
            _src_path = _lib_dir + _basename + _src_ext;
            _lib_path = _lib_dir + _basename + _lib_ext;
        }

        void open_lib() override {
            // Only do something if the library is not currently open
            if (!_open) {
                // RTLD_NOW, we could try RTLD_LAZY for performance
                if (!(_handle = ::dlopen(_lib_path.c_str(), RTLD_NOW))) {
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
        const std::string _basename;
        const std::string _lib_dir;
        const std::string _src_ext;
        const std::string _lib_ext;
        const std::string _alloc_sym;
        const std::string _del_sym;
        bool _open = false;

        std::string _lib_path;
        std::vector<string> _src_path;

        // Checks if the source file was modified
        bool was_modified() const {
            // Source and library file stat structs
            struct stat src_stat, lib_stat;
            const auto src_ret = ::stat(_src_path[0].c_str(), &src_stat) == 0;
            const auto lib_ret = ::stat(_lib_path.c_str(), &lib_stat) == 0;

            // If library file does not exist, we consider the source file to
            // have been modified. Otherwise, we return whether the source
            // is newer than the library based on mtime.
            if (src_ret && !lib_ret && errno == ENOENT) {
                return true;
            } else if (src_ret && lib_ret) {
                return src_stat.st_mtim.tv_sec > lib_stat.st_mtim.tv_sec;
            }

            return false;
        } 

        // Recompile if necessary
        bool recompile() {
            if (was_modified()) {
                // Fork and exec
                auto child_pid = ::fork();
                if (!child_pid) {
                    auto src_fname = _basename + _src_ext;
                    auto lib_fname = _basename + _lib_ext;
                    ::chdir(_lib_dir.c_str());
                    ::execlp("g++", "g++", src_fname.c_str(), "-std=c++17",
                                "-shared", "-fPIC", "-o", lib_fname.c_str(),
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