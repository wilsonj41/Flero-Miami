#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <cgicc/HTMLClasses.h>
#include <string>

#include "IframeExampleView.hpp"

using namespace fleropp::literals;
using namespace cgicc;
namespace htmls = fleropp::html_stream;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_iframe
INIT_VIEW(IframeExampleView)

void IframeExampleView::get(const fleropp::io::RequestData& request) {
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;

    std::string content_type("text/html");
    htmls::gen_html_boilerplate(content_type);

    body << h2("HTML Iframes")
        << p("An iframe is used to display a web page within a web page:")
        << iframe().set("src","https://editor.p5js.org/linr5/full/b97izcDlV").set("width","600").set("height","600");
    
    // close body and html tag
    body << htmls::gen_end_tag();
    html << body << htmls::gen_end_tag();
}