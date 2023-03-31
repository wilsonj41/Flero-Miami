#include <cstdlib>
#include <iterator>

#include "HelloWorldView.hpp"

#include <fleropp/HTMLLiterals.hpp>
#include <fleropp/QueryString.hpp>
#include <fleropp/RequestData.hpp>
#include <fleropp/PostFile.hpp>

#include <fleropp/SQLBuilder.h>

#include <soci/soci.h>
#include <soci/error.h>
#include <soci/session.h>

INIT_VIEW(HelloWorldView);

void HelloWorldView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    using namespace SQLBuilder; 

    // Examples derived from library repo
    // Insert
    InsertModel i;
    i.insert("score", 100)
            ("name", std::string("six"))
            ("age", (unsigned char)20)
            ("address", "beijing")
            ("create_time", nullptr)
        .into("user");

    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Hello, World!</title>"_h "</head>"_h;
        "<body>"_h;
            try {
                soci::session sql{"mysql", "host=db db=fleropp_test password=fleropp"};
            } catch (const soci::soci_error& ex) {
                "<h1>DB Status: {}</h1>"_f(ex.what());
            } catch (const std::runtime_error& ex) {
                "<h1>DB Status: {}</h1>"_f(ex.what());
            } catch (...) {
                "<h1>DB Status: {}</h1>"_f("Unknown exception");
            }
            "<h1>Hello, World2!</h1>"_h;
            "<h1>Hello, World&mdash;{}</h1>"_f("from me, in a template");
            "<h1>Hello, {}!</h1>"_f(request.get_query_string().get("name"));
            "<h1>User-Agent: {}</h1>"_f(request.get_header("User-Agent"));
            "<h1>Request type: {}</h1>"_f(request.method());
            "<h1>Random number: {}</h1>"_f(rand());
            "<h1>SQLBuilder Test</h1>"_h;
            "<span>{}<br></span>"_f(i.str());
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
    auto pstring = request.get_post_text();
    "<h1>Result: {}</h1>"_f(pstring.get("some_text"));
    "<h1>Request type: {}</h1>"_f(request.method());
}