#include "HelloWorldView.hpp"
#include "HTMLLiterals.hpp"

#include <string_view>

extern "C" {
    HelloWorldView *allocator() {
        return new HelloWorldView();
    }

    void deleter(HelloWorldView *ptr) {
        delete ptr;
    }

}

void HelloWorldView::generate(std::ostream &os) {
    using namespace fleropp_literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Hello, World</title>"_h "</head>"_h;
        "<body>"_h;
            "<h1>Hello, World!</h1>"_h;
        "</body>"_h;
    "</html>"_h;
}