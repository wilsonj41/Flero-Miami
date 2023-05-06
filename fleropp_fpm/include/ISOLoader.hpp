#ifndef I_SO_LOADER_HPP
#define I_SO_LOADER_HPP

#include <memory>

// fleropp_fpm project namespace
/**
 * \namespace fleropp::fpm
*/
namespace fleropp::fpm {
    /**
     * \class ISOLoader
     * \brief Interface that represents a shared object loader
     * \tparam T The type of the object that will be loaded from the shared object
     */
    template <class T> 
    class ISOLoader {
      public:
        /**
         * \brief Default destructor
         */
        virtual ~ISOLoader() = default;

        /**
         * \brief Loads the shared library into memory
         * 
         * This function loads the shared library into memory so that its symbols can
         * be accessed. This is a purely virtual function; it must be overriden in implementation.
         */
        virtual void open_lib() = 0;
        
        /**
         * \brief Unloads the shared library from memory
         * 
         * This function unloads the shared library from memory. 
         * This is a purely virtual function; it must be overridden in implementation.
         */
        virtual void close_lib() = 0;

        /**
         * \brief Returns a smart pointer to the shared object with reference 
         * counting
         * 
         * This function returns a std::shared_ptr<T> to the shared object.
         * This is a purely virtual function; it must be overridden in implementation.
         * 
         * \tparam Args Types of arguments passed to the constructor of the shared object
         * \param[in] args Arguments passed to the constructor of the shared object
         * \return A std::shared_ptr<T> to the shared object
         */
        template<typename... Args>
        std::shared_ptr<T> get_instance(Args... args) {
          return nullptr;
        }
    };
}

#endif /* I_SO_LOADER_HPP */