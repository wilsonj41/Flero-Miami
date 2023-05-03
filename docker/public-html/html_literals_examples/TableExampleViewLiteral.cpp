#include <cgicc/HTMLClasses.h>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <ranges>

#include "TableExampleViewLiteral.hpp"

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_basic

INIT_VIEW(TableExampleViewLiteral)

void TableExampleViewLiteral::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    namespace htmls = fleropp::html_stream;

    htmls::gen_html_boilerplate("text/html");
    "<html>"_h;
        "<body>"_h;
            "<h2>HTML Tables</h2>"_h;

            "<p>HTML tables start with a table tag.</p>"_h;
            "<p>Table rows start with a tr tag.</p>"_h;
            "<p>Table data start with a td tag.</p>"_h;

            "<hr>"_h;
            "<h2>1 Column:</h2>"_h;

            "<table>"_h;
                "<tr>"_h;
                    "<td>100</td>"_h;
                "<tr>"_h;
            "</table>"_h;

            "<hr>"_h;
            "<h2>1 Row and 3 Columns:</h2>"_h;
            "<table>"_h;
                "<tr>"_h;
                    for (auto i : std::views::iota(1)
                                | std::views::take(3)) {
                        "<td>{}</td>"_f(i * 100);
                    }
                "</td>"_h;
            "</table>"_h;

            "<hr>"_h;
            "<h2>3 Rows and 3 Columns:</h2>"_h;
            "<table>"_h;
                for (auto i : std::views::iota(0, 3)) {
                    "<tr>"_h;
                    for (auto j : std::views::iota((i * 3) + 1)
                                | std::views::take(3)) {
                        "<td>{}</td>"_f(j * 100);
                    }
                    "</tr>"_h;
                }
            "</table>"_h;

            "<hr>"_h;
        "</body>"_h;
    "</html>"_h;
}
