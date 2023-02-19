#ifndef I_SO_LOADER_HPP
#define I_SO_LOADER_HPP

#include <memory>

// fleropp_fpm project namespace
namespace fleropp::fpm {
    /**
     * Interface that represents a shared object loader. 
     */
    template <class T> 
    class ISOLoader {
      public:
        /**
         * Default destructor.
         */
        virtual ~ISOLoader() = default;

        /**
         * Loads the shared library into memory so that its symbols can
         * be accessed.
         * 
         * Pure virtual function; must be overridden in implementation.
         */
        virtual void open_lib() = 0;
        
        /**
         * Unloads the shared library from memory.
         * 
         * Pure virtual function; must be overridden in implementation.
         */
        virtual void close_lib() = 0;

        /**
         * Returns a smart pointer to the shared object with reference 
         * counting.
         * 
         * Pure virtual function; must be overridden in implementation.
         * 
         * \return A std::shared_ptr<T> to the shared object. 
         */
        virtual std::shared_ptr<T> get_instance() = 0;
    };
}

#endif /* I_SO_LOADER_HPP */