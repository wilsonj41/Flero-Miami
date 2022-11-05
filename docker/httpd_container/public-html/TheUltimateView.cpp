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
    using namespace fleropp_html_stream;
    using namespace fleropp_literals;
    HTMLStream<"<html>"> html();
    HTMLStream<"<body>"> body();


    "Content-type: text/html\r"_h;
    "\r"_h;

    body << cgicc::h1("Hello world").getData() << gen_end_tag();
    html << body << gen_end_tag();

}