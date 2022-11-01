#ifndef SCOPED_REDIRECT_HPP
#define SCOPED_REDIRECT_HPP

#include <iostream>
#include <memory>
#include <type_traits>

namespace fleropp_fpm {
  /**
   * \brief A wrapper class template implementing RAII for iostream 
   * redirection.
   * \headerfile ScopedRedirect.hpp
   */
  template<typename T1,
           typename T2, 
           typename = std::enable_if_t<std::is_base_of_v<std::ios_base, T1> && 
                                       std::is_base_of_v<std::ios_base, T2>>>
  class ScopedRedirect {
    public:
      /**
       * Unitary explicit constructor.
       * 
       * \param[in] from std::iostream to which `to` will be redirected.
       * \param[in,out] to std::iostream that will be redirected to `from`.
       */
      explicit ScopedRedirect(const T1 &from, T2 &to) 
                              : m_original_sbuf_ptr{to.rdbuf()},
                                m_from{from}, m_to{to} {
        m_to.rdbuf(m_from.rdbuf());
      }

      /**
       * Explicitly-defined destructor. Accomplishes RAII by returning `to`
       * to its original state.
       */
      ~ScopedRedirect() {
        m_to.rdbuf(m_original_sbuf_ptr);
      }

      // Delete the default c-tor
      ScopedRedirect() = delete;

      // Delete the implicit copy c-tor and copy assignment operator
      // Thus the class is neither copyable nor moveable
      ScopedRedirect(const ScopedRedirect&) = delete;
      ScopedRedirect& operator=(const ScopedRedirect&) = delete;

    private:
      std::streambuf *m_original_sbuf_ptr;
      const T1 &m_from;
      T2 &m_to;
  }; 
}

#endif /* SCOPED_REDIRECT_HPP */