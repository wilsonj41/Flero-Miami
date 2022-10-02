#ifndef SO_LOADER_HPP
#define SO_LOADER_HPP

#include "ISOLoader.hpp"

#include <iostream>
#include <memory>
#include <string>


#include <dlfcn.h>

namespace so_loader {
    template <class T> 
    class SOLoader : public ISOLoader<T> {
      private:
        void *_handle;
        std::string _lib_path;
        std::string _alloc_sym;
        std::string _del_sym;
        bool _open = false;

      public:
        SOLoader(const std::string &lib_path, 
                    const std::string &alloc_sym = "allocator", 
                    const std::string &del_sym = "deleter" ) :
                        _handle(nullptr), _lib_path(lib_path),
                        _alloc_sym(alloc_sym), _del_sym(del_sym) {}

        ~SOLoader() = default;

        void open_lib() {
            if (!(_handle = ::dlopen(_lib_path.c_str(), RTLD_NOW))) {
                std::cerr << ::dlerror() << '\n';
            } else {
                _open = true;
            }
        }

        void close_lib() {
            if (_open) {
                if (::dlclose(_handle) != 0) {
                    std::cerr << ::dlerror() << '\n';
                } else {
                    _open = false;
                }
            }
        }

        std::shared_ptr<T> get_instance() override {
            using alloc_fun_t = T *(*)();
            using del_fun_t = void (*)(T *);
            auto alloc_fun = reinterpret_cast<alloc_fun_t>(::dlsym(_handle, _alloc_sym.c_str()));
            auto del_fun = reinterpret_cast<del_fun_t>(::dlsym(_handle, _del_sym.c_str()));

            if (!alloc_fun || !del_fun) {
                close_lib();
                std::cerr << ::dlerror() << '\n';
            }

            return std::shared_ptr<T>(alloc_fun(), [del_fun] (T *p) { del_fun(p); });
        }
    };
}

#endif /* SO_LOADER_HPP */