#include "HelloWorldView.hpp"
#include <fleropp/HTMLLiterals.hpp>

extern "C" {
    HelloWorldView *allocator() {
        return new HelloWorldView();
    }

    void deleter(HelloWorldView *ptr) {
        delete ptr;
    }

}

void HelloWorldView::generate() {
    using namespace fleropp_literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Hello!, World</title>"_h "</head>"_h;
        "<body>"_h;
            "<h1>Hello, World!</h1>"_h;
            "<h1>Hello, World&mdash;{}</h1>"_f("from me, in a template");
        "</body>"_h;
    "</html>"_h;
}