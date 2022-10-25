#ifndef HTML_LITERALS_HPP
#define TEMPLATE_LITERALS_HPP

#include <iostream>

namespace fleropp_literals {
  void operator"" _h(const char* data, std::size_t len) {
    std::cout << data << '\n';
  };
}

#endif /* HTML_LITERALS_HPP */