#include "IdAttributeExampleView.hpp"
#include "HTMLStream.hpp"
#include "HTMLLiterals.hpp"
#include "cgicc/HTMLClasses.h"
#include <string>

using namespace fleropp_literals;
using namespace cgicc;
using namespace std;
namespace htmls = fleropp_html_stream;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_id_css

extern "C" {
    IdAttributeExampleView *allocator() {
        return new IdAttributeExampleView();
    }

    void deleter(IdAttributeExampleView *ptr) {
        delete ptr;
    }

}

void defineStyle(htmls::HTMLStream<"<style>","</style>"> &style) {
    style << "#myHeader { background-color: lightblue;" 
                    << "color: black;"  
                    << "text-align: center;" 
                    << "padding: 40px;"
                    << "}" << htmls::gen_end_tag();
}

void IdAttributeExampleView::generate() {
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<head>", "</head>"> head;
    htmls::HTMLStream<"<body>", "</body>"> body;

    htmls::HTMLStream<"<style>","</style>"> style;
    defineStyle(style);

    /* --------------- NOTE: Building the <body> tag. ---------------  */
    body << h2("The id Attribute") << p("Use CSS to style an element with the id \"myHeader\":")
        << h1("My Header").set("id","myHeader") << htmls::gen_end_tag();

    head << style << htmls::gen_end_tag();
    html << head << body << htmls::gen_end_tag();

}