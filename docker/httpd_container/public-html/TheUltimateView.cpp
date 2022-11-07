#include "TheUltimateView.hpp"
#include "HTMLStream.hpp"
#include "HTMLLiterals.hpp"
#include "cgicc/HTMLClasses.h"

extern "C" {
    TheUltimateView *allocator() {
        return new TheUltimateView();
    }

    void deleter(TheUltimateView *ptr) {
        delete ptr;
    }

}

void TheUltimateView::generate() {
    using namespace fleropp_literals;
    namespace htmls = fleropp_html_stream;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;
    htmls::HTMLStream<"<h1>", "</h1>"> h1;
    htmls::HTMLStream<"<h2>", "</h2>"> h2;
    htmls::HTMLStream<"<a href='https://www.youtube.com/watch?v=dQw4w9WgXcQ'>", "</a>"> sneaky; 


    "Content-type: text/html\r"_h;
    "\r"_h;
    "<!DOCTYPE html>"_h;

    h1 << "Hello, World" << htmls::gen_end_tag();
    h2 << "From inside <em>the ultimate</em> view generated using <code>HTMLStream</code>" << htmls::gen_end_tag();
    sneaky << "Click for free money!" << htmls::gen_end_tag();
    html << h1 << h2 << sneaky << htmls::gen_end_tag();

}