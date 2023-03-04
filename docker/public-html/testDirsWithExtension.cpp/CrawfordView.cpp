#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <cgicc/HTMLClasses.h>

#include "CrawfordView.hpp"

extern "C" {
    CrawfordView *allocator() {
        return new CrawfordView();
    }

    void deleter(CrawfordView *ptr) {
        delete ptr;
    }

}

void CrawfordView::generate() {
    using namespace fleropp_literals;
    namespace htmls = fleropp_html_stream;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;
    htmls::HTMLStream<"<h2>", "</h2>"> h2;

    std::string content_type("text/html");
    htmls::gen_html_boiler_plate(content_type);

    h2 << "<em>Welcome to the Craw Zone</em>" << htmls::gen_end_tag();
    body << h2;
    for (int i = 0; i < 200; i++) {
        body << cgicc::img().set("src", "images/crawford.jpg").set("width", "60").set("height", "60");
        body << "\n";
    }

    html << body << htmls::gen_end_tag();
}