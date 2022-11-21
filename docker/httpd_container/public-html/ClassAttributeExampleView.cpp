#include "ClassAttributeExampleView.hpp"
#include "HTMLStream.hpp"
#include "HTMLLiterals.hpp"
#include "cgicc/HTMLClasses.h"
#include <string>
#include <vector>
#include <tuple>

using namespace fleropp_literals;
using namespace cgicc;
using namespace std;
namespace htmls = fleropp_html_stream;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_classes_capitals
// Note: Inline javascript seems to only work when it is not inside of a header.

extern "C" {
    ClassAttributeExampleView *allocator() {
        return new ClassAttributeExampleView();
    }

    void deleter(ClassAttributeExampleView *ptr) {
        delete ptr;
    }

}

void defineStyle(htmls::HTMLStream<"<style>","</style>"> &style) {
    style << ".city { background-color: tomato;" 
                    << "color: white;" 
                    << "border: 2px solid black;" 
                    << "margin: 20px;" 
                    << "padding: 20px;"
                    << "}" << htmls::gen_end_tag();
}

void ClassAttributeExampleView::generate() {
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<head>", "</head>"> head;
    htmls::HTMLStream<"<body>", "</body>"> body;

    htmls::HTMLStream<"<style>","</style>"> style;
    defineStyle(style);

    htmls::HTMLStream<"<div class=\"city\">","</div>"> div1;
    div1 << h2("London") << p("London is the capital of England") << htmls::gen_end_tag();

    htmls::HTMLStream<"<div class=\"city\">","</div>"> div2;
    div2 << h2("Paris") << p("Paris is the capital of France") << htmls::gen_end_tag();

    htmls::HTMLStream<"<div class=\"city\">","</div>"> div3;
    div3 << h2("Tokyo") << p("Tokyo is the capital of Japan") << htmls::gen_end_tag();

    body << div1 << div2 << div3 << htmls::gen_end_tag();
    head << style << htmls::gen_end_tag();

    html << head << body << htmls::gen_end_tag();

}