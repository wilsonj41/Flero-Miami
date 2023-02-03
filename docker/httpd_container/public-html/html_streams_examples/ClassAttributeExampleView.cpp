#include <cgicc/HTMLClasses.h>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <string>
#include <unordered_map>

#include "ClassAttributeExampleView.hpp"

using namespace fleropp_literals;
using namespace cgicc;
using namespace std;
namespace htmls = fleropp_html_stream;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_classes_capitals
// Note: Inline javascript seems to only work when it is not inside of a header.


INIT_VIEW(ClassAttributeExampleView)

namespace class_attr_example_util {
    void defineStyle(htmls::HTMLStream<"<style>","</style>"> &style) {
        style << ".city { background-color: tomato;" 
                        << "color: white;" 
                        << "border: 2px solid black;" 
                        << "margin: 20px;" 
                        << "padding: 20px;"
                        << "}" << htmls::gen_end_tag();
    }
}

void ClassAttributeExampleView::generate() {
    using namespace class_attr_example_util;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<head>", "</head>"> head;
    htmls::HTMLStream<"<body>", "</body>"> body;

    htmls::HTMLStream<"<style>", "</style>"> style;
    defineStyle(style);

    std::unordered_map<std::string, std::string> capitals = 
            { {"London", "London is the capital of England"},
              {"Paris", "Paris is the capital of France"},
              {"Tokyo", "Tokyo is the capital of Japan"}};

    for(auto [key, value] : capitals) {
        htmls::HTMLStream<"<div class=\"city\">","</div>"> div;
        div << h2(key) << p(value) << htmls::gen_end_tag();
        body << div;  
    }

    body << htmls::gen_end_tag();
    head << style << htmls::gen_end_tag();

    html << head << body << htmls::gen_end_tag();

}