#ifndef SCOPED_REDIRECT_HPP
#define SCOPED_REDIRECT_HPP

#include <iostream>
#include <memory>
#include <type_traits>

namespace fleropp::io {
  /**
   * \brief A wrapper class template implementing RAII for iostream 
   * redirection.
   * \headerfile ScopedRedirect.hpp
   */
  template<typename From, typename To>
  class ScopedRedirect {
    public:
      /**
       * Constructor.
       * 
       * \param[in] from std::iostream to which `to` will be redirected.
       * \param[in,out] to std::iostream that will be redirected to `from`.
       */
      ScopedRedirect(From &from, To &to) 
                              : m_original_sbuf_ptr{to.rdbuf()},
                                m_from{from}, m_to{to} {
        if constexpr (!std::is_base_of_v<std::streambuf, From>) {
          m_to.rdbuf(m_from.rdbuf());
        } else {
          m_to.rdbuf(&m_from);
        }
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
      std::streambuf* m_original_sbuf_ptr;
      From& m_from;
      To& m_to;
  }; 
}

#endif /* SCOPED_REDIRECT_HPP */