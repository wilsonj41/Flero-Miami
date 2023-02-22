#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <cgicc/HTMLClasses.h>
#include <string>

#include "TableExampleView.hpp"

using namespace fleropp::literals;
using namespace cgicc;
namespace htmls = fleropp::html_stream;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_basic

INIT_VIEW(TableExampleView)

namespace table_example_util {
  void firstBuildTable(htmls::HTMLStream<"<table>","</table>"> &table1) {
      htmls::HTMLStream<"<tr>","</tr>"> tr;

      tr << td("100") << htmls::gen_end_tag();
      table1 << tr << htmls::gen_end_tag();
  }

  void secondBuildTable(htmls::HTMLStream<"<table>","</table>"> &table2) {
      htmls::HTMLStream<"<tr>","</tr>"> tr;

      for (int i = 100; i < 400; i=i+100) {
        std::string s = std::to_string(i);
        tr << td(s);
      }

      tr << htmls::gen_end_tag();
      table2 << tr << htmls::gen_end_tag();
  }

  void thirdBuildTable(htmls::HTMLStream<"<table>","</table>"> &table3) {
      htmls::HTMLStream<"<tr>","</tr>"> tr1; // 100 200 300
      htmls::HTMLStream<"<tr>","</tr>"> tr2; // 400 500 600
      htmls::HTMLStream<"<tr>","</tr>"> tr3; // 700 800 900

      for (int i = 100; i < 1000; i=i+100) {
        if (i >= 100 && i <= 300) {
          tr1 << td(std::to_string(i));
        }
        else if (i >= 400 && i <= 600) {
          tr2 << td(std::to_string(i));
        }
        else {
          tr3 << td(std::to_string(i));
        }
      }

      // closing all ray tags
      tr1 << htmls::gen_end_tag();
      tr2 << htmls::gen_end_tag();
      tr3 << htmls::gen_end_tag();

      // add to table3
      table3 << tr1 << tr2 << tr3 << htmls::gen_end_tag();
  }
}

void TableExampleView::generate() {
    using namespace table_example_util;
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;

    std::string content_type("text/html");
    htmls::gen_html_boiler_plate(content_type);

    /* --------------- NOTE: All the tables are built first before adding into the body. ---------------  */
    // builds the first table
    htmls::HTMLStream<"<table>","</table>"> table1;
    firstBuildTable(table1);

    // builds the second table
    htmls::HTMLStream<"<table>","</table>"> table2;
    secondBuildTable(table2);

    // builds the third table
    htmls::HTMLStream<"<table>","</table>"> table3;
    thirdBuildTable(table3);

    /* --------------------- NOTE: Building the <body> tag of the webpage ---------------------  */
    body << h2("HTML Tables")
        << p("HTML tables start with a table tag 40.")
        << p("Table rows start with a tr tag.")
        << p("Table data start with a td tag.");
    
    // add all the tables to the body tag.
    body << hr() << table1 << hr() << table2 << hr() << table3;




    // close off the body and html tags when done.
    body << htmls::gen_end_tag();
    html << body << htmls::gen_end_tag();



}