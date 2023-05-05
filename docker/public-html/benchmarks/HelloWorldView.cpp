#include "HelloWorldView.hpp"
#include <fleropp/HTMLLiterals.hpp>

INIT_VIEW(HelloWorldView)

void HelloWorldView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    "Content-Type: text/html\r"_h;
    "\r"_h;
    "<h1>Hello, World!</h1>"_h;
}
