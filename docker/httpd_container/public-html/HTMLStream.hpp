#ifndef HTML_STREAM_HPP
#define HTML_STREAM_HPP

#include "FleroppIO.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <array>
#include <string_view>

namespace fleropp_html_stream {

  template<size_t N>
  struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
      std::copy_n(str, N, value);
    }
    char value[N];
  };

  static struct EndTag{} end_tag;

  EndTag gen_end_tag() {
    return {};
  }

  //---------------------- This is the general element tag. ----------------------------
  template <StringLiteral Tag>
  class HTMLStream : public std::ostream {
    friend HTMLStream &operator<<(HTMLStream &html_stream, std::string &content) {
      m_ss << content;
      return html_stream;
    }
    
    friend HTMLStream &operator<<(HTMLStream &html_stream, EndTag end_tag) {
      std::string str_end_tag = m_tag;
      str_end_tag.insert(1, "/");

      if constexpr(Tag.value != "<html>") {
          m_ss << str_end_tag << "\n";
      } else {
          fleropp_io:fppout << m_ss.str() << "\n";
      }
      return html_stream;
    }

    friend HTMLStream &operator<<(HTMLStream &html_stream1, HTMLStream &html_stream2) {
      html_stream1.m_ss << html_stream2.m_ss.str();
      return html_stream1;
    }
  
  public:
    HTMLStream() : m_ss{m_tag} {}

  private:
    std::stringstream m_ss(Tag.value);
    static constexpr std::string_view m_tag(Tag.value);
  };  
}



#endif
