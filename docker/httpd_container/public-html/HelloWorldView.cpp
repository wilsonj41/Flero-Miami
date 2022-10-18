#include "HelloWorldView.hpp"

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
    // Compile-time / runtime tradeoff
    // This will be slower to compile, but faster at runtime
    // Very little difference at this scale, though.
    static constexpr auto content = 
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>\n"
        "  <head>\n"
        "    <title>Hello, World Can i get a bruh</title>\n"
        "  </head>\n"
        "  <body>\n"
        "    <h1>fjdkslajfdkalsj</h1>\n"
        "  </body>\n"
        "</html>\n";
    os << content;
}