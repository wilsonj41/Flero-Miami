#ifndef I_SO_LOADER_HPP
#define I_SO_LOADER_HPP

#include <memory>

namespace so_loader {
    template <class T> 
    class ISOLoader {
      public:
        virtual ~ISOLoader() = default;
        virtual void open_lib() = 0;
        virtual std::shared_ptr<T> get_instance() = 0;
        virtual void close_lib() = 0;
    };
}

#endif /* I_SO_LOADER_HPP */