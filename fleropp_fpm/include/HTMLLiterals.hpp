#ifndef HTML_LITERALS_HPP
#define HTML_LITERALS_HPP
/**
 * \file HTMLLiterals.hpp
 * \brief Flero++ HTML UDLs.
 * 
 * This file contains the header-only Flero++ user-defined HTML literals
 * library, along with a class `FormatProxy` that allows format strings
 * to be passed to a `_f` format literal.
 */

#include "FleroppIO.hpp"

#if __cpp_lib_format >= 202110L
#include <format>
namespace fmt = std;
#else
//#define FMT_HEADER_ONLY
#include <fmt/format.h>
#endif

#include <iostream>
#include <vector>

/**
 * \namespace fleropp::literals
 */
namespace fleropp::literals {
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
       * Variadic function template for `operator()`. Formats the string
       * passed to the c-tor using a `printf` style list of arguments.
       * 
       * \param[in] args The format arguments.
       */
      template<typename... Args>
      void operator()(Args&& ...args) {
        fleropp::io::fppout << fmt::format(fmt::runtime(m_data), std::forward<Args>(args)...) << '\n';
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
  FormatProxy operator"" _f(const char* data, [[maybe_unused]] std::size_t len) {
    return {data};
  }
  
  /**
   * Free function UDL that does not accept template format arguments.
   * 
   * \param[in] data Pointer to contents of string literal (implicit parameter).
   * \param[in] len Length of string literal (implicit parameter).
   */
  void operator"" _h(const char* data, [[maybe_unused]] std::size_t len) {
    fleropp::io::fppout << data << '\n';
  }
}

#endif /* HTML_LITERALS_HPP */