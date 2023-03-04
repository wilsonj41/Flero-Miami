#include <cgicc/HTMLClasses.h>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <ranges>

#include "IframeExampleViewLiteral.hpp"

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_basic

INIT_VIEW(IframeExampleViewLiteral)

void IframeExampleViewLiteral::generate() {
    using namespace fleropp::literals;
    namespace htmls = fleropp::html_stream;

    htmls::gen_html_boiler_plate("text/html");
    "<html>"_h;
        "<body>"_h;
            "<iframe src='https://projects.baxterhuntington.xyz/modulon' width=600 height=600/>"_h;
        "</body>"_h;
    "</html>"_h;
}
