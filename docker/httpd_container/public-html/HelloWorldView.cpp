#include "HelloWorldView.hpp"

#include <string_view>
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace cgicc;
using namespace std;

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

   os << HTTPHTMLHeader() << HTMLDoctype( HTMLDoctype::eStrict ) << endl
        << html().set( "lang", "en" ).set( "d ir", "ltr" ) << endl;

    os << head() << endl
        << title("Hello, World")
        << head() << endl;
    
    os << body() << endl
        << h1("Hello, world!") << endl
        << body() << endl;
    
    os << html();

    
    /*static constexpr auto content = 
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>\n"
        "  <head>\n"
        "    <title>Hello, World</title>\n"
        "  </head>\n"
        "  <body>\n"
        "    <h1>Hello, World!</h1>\n"
        "  </body>\n"
        "</html>\n";
    os << content; */
}