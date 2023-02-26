#include <cstdlib>
#include <iterator>

#include "MultiPartPostView.hpp"

#include <fleropp/HTMLLiterals.hpp>
#include <fleropp/QueryString.hpp>
#include <fleropp/RequestData.hpp>
#include <fleropp/HttpPostData.h>

INIT_VIEW(MultiPartPostView);

void MultiPartPostView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Hello!, World</title>"_h "</head>"_h;
        "<body>"_h;
            "<h1>Hello, World2!</h1>"_h;
            "<h1>Hello, World&mdash;{}</h1>"_f("from me, in a template");
            "<h1>Hello, {}!</h1>"_f(request.get_query_string().get("name"));
            "<h1>User-Agent: {}</h1>"_f(request.get_header("User-Agent"));
            "<h1>Request type: {}</h1>"_f(request.method());
            "<h1>Random number: {}</h1>"_f(rand());
            "<form action='hello.elf' method='post' enctype='multipart/form-data' target='out_iframe'>"_h;
                "Name: <input type='text' name='person'><br>"_h;
                "File: <input type='file' name='secret'><br>"_h;
                "<input type='submit'>"_h;
            "</form>"_h;
            "<iframe name='out_iframe'></iframe>"_h;
        "</body>"_h;
    "</html>"_h;
}

void MultiPartPostView::post(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    std::vector<HttpPostData> dataVec = request.get_post_data();
    auto name1 = dataVec[0].data;
    std::string result(name1.begin(),name1.end());
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<h1>Result: {}</h1>"_f(result);
    "<h1>Request type: {}</h1>"_f(request.method());
}