#include "TheUltimateView.hpp"
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <cgicc/HTMLClasses.h>

INIT_VIEW(TheUltimateView);

void TheUltimateView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    namespace htmls = fleropp::html_stream;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;
    htmls::HTMLStream<"<h2>", "</h2>"> h2;
    htmls::HTMLStream<"<a href='https://www.youtube.com/watch?v=dQw4w9WgXcQ'>", "</a>"> sneaky; 


    // "Content-type: text/html\r"_h;
    // "\r"_h;
    // "<!DOCTYPE html>"_h;
    std::string content_type("text/html");
    htmls::gen_html_boiler_plate(content_type);

    h2 << "From inside <em>the ultimate</em> view generated using <code>HTMLStream</code>" << htmls::gen_end_tag();
    sneaky << "Click for free money!" << htmls::gen_end_tag();
    body << cgicc::h1("Hello world, I'm walter yo!").set("id","walter") << cgicc::img().set("src","images/walter.jpg").set("alt", "A dog named walter staring at you.");
    body << cgicc::div(cgicc::p("I am inside a div!!")).set("id","indiv");
    body << h2 << sneaky << htmls::gen_end_tag();
    html << body << htmls::gen_end_tag();

}