#ifndef HTML_STREAM_HPP
#define HTML_STREAM_HPP

#include "FleroppIO.hpp"
#include "HTMLLiterals.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include "cgicc/HTMLClasses.h"
#include "cgicc/MStreamable.h"

// fleropp HTML stream namespace
namespace fleropp_html_stream {

  /**
   * \brief Class that represents a string literal that is usable as a non-type
   * template parameter. Per the C++ Standard, a non-type template parameter
   * must have "structural type", hence we create this class representing a
   * string that fits those requirements:
   *    - all base classes and non-static data members are public and 
   *      non-mutable and  
   *    - the types of all base classes and non-static data members
   *      are structural types or (possibly multi-dimensional) array thereof. 
   * \tparam N Length of the string literal.
   */  
  template<std::size_t N>
  struct StringLiteral {
    /**
     * Constant expression c-tor.
     * Accepts a reference to avoid decay of array to pointer.
     * 
     * \param[in] str Reference to a const char[N] containing the string data.
     */
    constexpr StringLiteral(const char (&str)[N]) {
      // Copy to N - 1 to strip null terminator
      // This overload of std::copy_n is constexpr in >= c++20
      std::copy_n(std::cbegin(str), N - 1, std::begin(value));
    }
    std::array<char, N - 1> value;
  };

  /**
   * \brief Tag type for use in tag dispatching akin to the implementation of
   * `std::setw` in the STL.
   */
  static struct EndTag{} end_tag;
  
  /**
   * Free function that returns an `EndTag` tag type, leading to the correct
   * overload of `operator<<` being resolved.
   */
  EndTag gen_end_tag() { return {}; }

  // TODO(linr5): Make `Content-type:` free function

  /**
   * Free function that generates the content type for the HTML page.
   * \param[in] type the content type.
  */
  void gen_content_type(std::string &content_type) {
    fleropp_io::fppout << "Content-type: " << content_type << "\r\n\r\n";
  }

  /**
   * Free function that generates the html DOCTYPE.
  */
  void gen_html_doctype() {
    fleropp_io::fppout << "<!DOCTYPE html> \n";
  }

  /**
   * Free function that generates the "boiler plate" for a HTML page.
   * \param[in] type the content type for content type header.
  */
  void gen_html_boiler_plate(std::string &content_type) {
    gen_content_type(content_type);
    fleropp_io::fppout << "\r\n";
    gen_html_doctype();
  }
  
  /**
   * \brief Tag type for use in template tag dispatching. Results in 
   * `HTMLStream` dumping output to global ostream upon receipt of
   * `EndTag`.
   */
  typedef struct DumpOnEnd dump_on_end;
  
  /**
   * \brief Class template that represents an output stream designed with HTML
   * `div`s in mind. 
   * 
   * \tparam StartT String representation of the start tag (e.g., "<html>").
   * \tparam EndT String representation of the end tag (e.g., "</html>").
   * \tparam EndTagAction (optional) Tag type indicating behavior on receipt
   * of `EndTag` from `gen_end_tag` (default = void).
   */
  template <
    StringLiteral StartT, 
    StringLiteral EndT, 
    typename EndTagAction = void
  >
  class HTMLStream {  
    public:
      /**
       * Default c-tor.
       */
      HTMLStream() { m_ss << m_start_tag << '\n'; }
      
      /**
       * Accessor for a pointer to the interal `streambuf`.
       * 
       * \return Pointer to the internal `streambuf`
       */
      auto* rdbuf() const { return m_ss.rdbuf(); }

      /**
       * Accessor to the internal `stringstream` string. 
       * 
       * \return String build from the `stringstream`.
       */
      auto str() const { return m_ss.str(); }

      /**
       * Accessor to the internal stringstream object. 
       * 
       * \return Reference to the internal `stringstream`.
       */
      auto& ss() { return m_ss; }
      
      /**
       * Friend stream insertion operator for use with strings.
       * 
       * \param[in, out] html_stream Stream to be written to.
       * \param[in] content String to be written to the `HTMLStream`.
       * 
       * \return A reference to `html_stream`.
       */
      friend auto& operator<<(HTMLStream &html_stream, const std::string &content) {
        html_stream.m_ss << content;
        return html_stream;
      }

      /**
       * Friend stream inseration operator for use with any class
       * in the cgicc library that implements MStreamable.
       * 
       * \param[in, out] html_stream Stream to be written to.
       * \param[in] content String to be writen to the `HTMLStream`.
       * 
       * \return A reference to `html_stream`.
      */
      friend auto& operator<<(HTMLStream &html_stream, const cgicc::MStreamable &content_html) {
        html_stream.m_ss << content_html;
        return html_stream;
      }

      /* friend auto& operator<<(HTMLStream &html_stream, cgicc::MStreamable &&contentHTML) {
        html_stream.m_ss << contentHTML;
        return html_stream;
      } */
      
      /**
       * Friend stream insertion operator that recieves end tag. The behavior of this
       * function depends on `std::is_same<EndTagAction, X>::value`, for some tag type
       * `X`. For example, `std::is_same_v<EndTagAction, DumpOnEnd>`, the stream will
       * be written to the global output stream upon receipt of the end tag. By default,
       * nothing is done upon receipt of the end tag. 
       * 
       * \param[in, out] html_stream Stream to be written to.
       * \param[in] end_tag An EndTag object generated by `gen_end_tag`.
       * 
       * \return A reference to `html_stream`.
       */
      friend auto& operator<<(HTMLStream &html_stream, const EndTag &end_tag) { 
        // Alternative to explicit specialization, we modify behavior based on the
        // type of an optional template parameter using the std::is_same 
        // metafunction
        if constexpr (std::is_same_v<EndTagAction, DumpOnEnd>) {
          fleropp_io::fppout << html_stream.m_ss.rdbuf() << html_stream.m_end_tag << '\n';
        } else {
          html_stream.m_ss << m_end_tag << '\n';
        }
        return html_stream;
      }

    private:
      // These can be static constexpr, as the start and end tags are determined
      // at compile time as part of the compiler's template instantiation
      // process; they are shared accross all instances of particular template
      // specialization.
      static constexpr std::string_view m_start_tag{std::cbegin(StartT.value), 
                                                    std::cend(StartT.value)}, 
                                        m_end_tag{std::cbegin(EndT.value),
                                                  std::cend(EndT.value)};
      std::stringstream m_ss;
  }; 

  /**
   * Free function stream insertion operator that receives another `HTMLStream`
   * of the with the same *or* different type signature.
   * 
   * \tparam StartT1 Start tag for the lhs `HTMLStream`.
   * \tparam EndT1 End tag for the lhs `HTMLStream`.
   * \tparam StartT2 Start tag for the rhs `HTMLStream`.
   * \tparam EndT2 End tag for the rhs `HTMLStream`.
   * \tparam U Dummy type for `EndTagAction` on lhs.
   * \tparam V Dummy type for `EndTagAction` on rhs.
   * 
   * \param[in, out] html_stream1 Stream to be written to.
   * \param[in] html_stream2 Stream to read from.
   * 
   * \return A reference to `html_stream1` (of type `decltype(html_stream1)&`).
   */
  template <
    StringLiteral StartT1,
    StringLiteral EndT1,
    StringLiteral StartT2, 
    StringLiteral EndT2,
    typename U,
    typename V
  >
  auto& operator<<(HTMLStream<StartT1, EndT1, U> &html_stream1,
                    const HTMLStream<StartT2, EndT2, V> &html_stream2) {
    html_stream1.ss() << html_stream2.rdbuf();
    return html_stream1;
  }
}

#endif /* HTML_STREAM_HPP */