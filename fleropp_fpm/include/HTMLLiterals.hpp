#ifndef HTML_LITERALS_HPP
#define HTML_LITERALS_HPP

#include "FleroppIO.hpp"

//#if __cpp_lib_format
//#include <format>
//namespace fmt = std;
//#else
//#define FMT_HEADER_ONLY
#include <fmt/format.h>
//#endif

#include <iostream>
#include <vector>

// fleropp literal namespace
namespace fleropp_literals {

  /**
   * \brief Simple proxy class to enable format strings in UDLs.
   */
  class FormatProxy {
    public:
      /**
       * Copy c-tor.
       * 
       * \param[in] data The string to be formatted.
       */
      FormatProxy(const std::string &data) : m_data{data} {};   

      /**
       * Move c-tor. 
       * 
       * \param[in] data The string to be formatted.
       */
      FormatProxy(std::string&& data) : m_data{std::move(data)} {};

      /**
       * Variadic function template for \code operator(). Formats the string
       * passed to the c-tor using a \code printf style list of arguments.
       * 
       * \param[in] args The format arguments.
       */
      template<typename... Args>
      void operator()(Args&& ...args) {
        fleropp_io::fppout << fmt::format(fmt::runtime(m_data), std::forward<Args>(args)...) << '\n';
      } 
    private:
      std::string m_data;
  };

  /**
   * Free function UDL that accepts template format arguments.
   * 
   * \param[in] data Pointer to contents of string literal (implicit parameter).
   * \param[in] len Length of string literal (implicit parameter).
   */
  FormatProxy operator"" _f(const char* data, std::size_t len) {
    return {data};
  }
  
  /**
   * Free function UDL that does not accept template format arguments.
   * 
   * \param[in] data Pointer to contents of string literal (implicit parameter).
   * \param[in] len Length of string literal (implicit parameter).
   */
  void operator"" _h(const char* data, std::size_t len) {
    fleropp_io::fppout << data << '\n';
  }
}

#endif /* HTML_LITERALS_HPP */