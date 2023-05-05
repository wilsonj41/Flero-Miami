#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <cgicc/HTMLClasses.h>
#include <string>
#include <vector>
#include <tuple>

#include "TableExampleWithBorderView.hpp"

using namespace fleropp::literals;
using namespace cgicc;
using namespace std;
namespace htmls = fleropp::html_stream;
using entry = std::tuple<std::string, std::string, int>;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_collapse

INIT_VIEW(TableExampleWithBorderView)

namespace table_border_example_util {
    void defineStyle(htmls::HTMLStream<"<style>","</style>"> &style) {
        style << "table, th, td { border: 1px solid black; border-collapse: collapse; }" << htmls::gen_end_tag();
    }

    void buildTable(htmls::HTMLStream<"<table>","</table>"> &table, const std::vector<entry> &list) {
        // the head of the table
        htmls::HTMLStream<"<tr>", "</tr>"> tr1;
        tr1 << th("Firstname") << th("Lastname") << th("Age") << htmls::gen_end_tag();
        table << tr1;

        // the content of the table
        for(int i = 0; i < 3; i++) {
            entry e = list[i];
            htmls::HTMLStream<"<tr>", "</tr>"> tr;
            tr << td(std::get<0>(e)) << td(std::get<1>(e)) << td(std::to_string(std::get<2>(e))) << htmls::gen_end_tag();
            table << tr;
        }

        table << htmls::gen_end_tag();
    }
}

void TableExampleWithBorderView::get(const fleropp::io::RequestData& request) {
    using namespace table_border_example_util;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<head>", "</head>"> head;
    htmls::HTMLStream<"<body>", "</body>"> body;

    htmls::HTMLStream<"<style>","</style>"> style;
    defineStyle(style);

    /* --------------------- NOTE: Building the <table> tag ---------------------  */
    // making an vector that has a tuple that stores (firstname, lastname, and age)
    std::vector<entry> list = {entry{"Jill","Smith",50}, entry{"Eve","Jackson",94}, entry{"John","Doe",80}};
    // build the table
    htmls::HTMLStream<"<table>","</table>"> table;
    buildTable(table, list);

    std::string content_type("text/html");
    htmls::gen_html_boilerplate(content_type);

    /* --------------------- NOTE: Building the <body> tag of the webpage ---------------------  */
    // the style tag is added to the head tag.
    head << style << htmls::gen_end_tag();

    body << h2("Collapsed Borders")
        << p("If you want the borders to collapse into one border, add the CSS border-collapse property.")
        << table
        << htmls::gen_end_tag();
    
    html << head << body << htmls::gen_end_tag();
}