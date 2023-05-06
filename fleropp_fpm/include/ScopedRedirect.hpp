#ifndef SCOPED_REDIRECT_HPP
#define SCOPED_REDIRECT_HPP

#include <iostream>
#include <memory>
#include <type_traits>

namespace fleropp::io {
  /**
   * \brief A wrapper class template that implements RAII for iostream
   * redirection
   * \headerfile ScopedRedirect.hpp
   */
  template <typename From, typename To>
  class ScopedRedirect {
  public:
    /**
     * \brief ScopedRedirect constructor
     *
     * \param[in] from std::iostream to which `to` will be redirected
     * \param[in,out] to std::iostream that will be redirected to `from`
     */
    ScopedRedirect(From &from, To &to)
        : m_original_sbuf_ptr{to.rdbuf()},
          m_from{from}, m_to{to} {
      if constexpr (!std::is_base_of_v<std::streambuf, From>)
      {
        m_to.rdbuf(m_from.rdbuf());
      }
      else
      {
        m_to.rdbuf(&m_from);
      }
    }

    /**
     * \brief ScopedRedirect destructor
     *
     * Explicitly-defined destructor. Accomplishes RAII by returning `to`
     * to its original state.
     */
    ~ScopedRedirect() {
      m_to.rdbuf(m_original_sbuf_ptr);
    }

    /**
     * \brief Delete the default c-tor
     */
    ScopedRedirect() = delete;

    /**
     * \brief ScopedRedirect copy constructor
     * \param[in] other ScopredRedirect object to copy from
     */
    ScopedRedirect(const ScopedRedirect &) = delete;

    /**
     * \brief ScopedRedirect copy assignment operator
     * \param[in] & ScopedRedirect object to assign from
     * \return Reference to this ScopedRedirect object
     */
    ScopedRedirect &operator=(const ScopedRedirect &) = delete;

  private:
    std::streambuf *m_original_sbuf_ptr;
    From &m_from;
    To &m_to;
  };
}

#endif /* SCOPED_REDIRECT_HPP */