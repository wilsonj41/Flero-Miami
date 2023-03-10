#include <cgicc/HTMLClasses.h>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>

#include "StatelessPostExample.h"

INIT_VIEW(StatelessPostExample)


void StatelessGetExample::post(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    namespace htmls = fleropp::html_stream;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>"
}
