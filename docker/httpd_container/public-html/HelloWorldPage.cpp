#include "HelloWorldPage.hpp"

extern "C" {
    HelloWorldPage *allocator() {
        return new HelloWorldPage();
    }

    void deleter(HelloWorldPage *ptr) {
        delete ptr;
    }

}

void HelloWorldPage::generate(std::ostream &os) {
    os << "Content-type: text/html\r\n"
        << "\r\n"
        << "<html>\n"
        << "  <head>\n"
        << "    <title>Hello, World</title>\n"
        << "  </head>\n"
        << "  <body>\n"
        << "    <h1>Hello, World!</h1>\n"
        << "  </body>\n"
        << "</html>\n";
}