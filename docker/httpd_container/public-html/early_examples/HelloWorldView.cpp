#include <cstdlib>
#include <iterator>

#include "HelloWorldView.hpp"

#include <fleropp/HTMLLiterals.hpp>
#include <fleropp/QueryString.hpp>
#include <fleropp/RequestData.hpp>

extern "C" {
    HelloWorldView *allocator() {
        return new HelloWorldView();
    }

    void deleter(HelloWorldView *ptr) {
        delete ptr;
    }

}

void HelloWorldView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Hello, World!!!</title>"_h "</head>"_h;
        "<body>"_h;
            "<h1>Hello, World!</h1>"_h;
            "<h1>Hello, World&mdash;{}</h1>"_f("from me, in a template");
            "<h1>Hello, {}!</h1>"_f(request.get_query_string().get("name"));
            "<h1>User-Agent: {}</h1>"_f(request.get_header("User-Agent"));
            "<h1>Request type: {}</h1>"_f(request.method());
            "<h1>Random number: {}</h1>"_f(rand());
            "<form action='hello.elf' method='post' target='out_iframe'>"_h;
                "<input type='text' name='some_text' value='florp'>"_h;
                "<input type='submit'>"_h;
            "</form>"_h;
            "<iframe name='out_iframe'></iframe>"_h;
        "</body>"_h;
    "</html>"_h;
}

void HelloWorldView::post(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    std::string form_data; 
    fleropp::io::fppin >> form_data;
    fleropp::io::QueryString qstring{form_data};
    qstring.parse(); 
    "<h1>Result: {}</h1>"_f(qstring.get("some_text"));
    "<h1>Request type: {}</h1>"_f(request.method());
}