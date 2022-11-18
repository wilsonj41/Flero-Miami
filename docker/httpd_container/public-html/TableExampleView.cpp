#include "TableExampleView.hpp"
#include "HTMLStream.hpp"
#include "HTMLLiterals.hpp"
#include "cgicc/HTMLClasses.h"
#include <string>

extern "C" {
    TableExampleView *allocator() {
        return new TableExampleView();
    }

    void deleter(TableExampleView *ptr) {
        delete ptr;
    }

}

/* 
<!DOCTYPE html>
<html>
<body>

<h2>HTML Tables</h2>

<p>HTML tables start with a table tag.</p>
<p>Table rows start with a tr tag.</p>
<p>Table data start with a td tag.</p>

<hr>
<h2>1 Column:</h2>

<table>
  <tr>
    <td>100</td>
  </tr>
</table>

<hr>
<h2>1 Row and 3 Columns:</h2>
<table>
  <tr>
    <td>100</td>
    <td>200</td>
    <td>300</td>
  </tr>
</table>

<hr>
<h2>3 Rows and 3 Columns:</h2>
<table>
  <tr>
    <td>100</td>
    <td>200</td>
    <td>300</td>
  </tr>
  <tr>
    <td>400</td>
    <td>500</td>
    <td>600</td>
  </tr>
  <tr>
    <td>700</td>
    <td>800</td>
    <td>900</td>
  </tr>
</table>

<hr>

</body>
</html>
*/


void TableExampleView::generate() {
  using namespace fleropp_literals;
  using namespace cgicc;
  namespace htmls = fleropp_html_stream;

    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;

    std::string content_type("text/html");
    htmls::gen_html_boiler_plate(content_type);

    // builds the first table
    htmls::HTMLStream<"<table>","</table>"> table1;
    htmls::HTMLStream<"<tr>","</tr>"> tr1;

    tr1 << td("100") << htmls::gen_end_tag();
    table1 << tr1 << htmls::gen_end_tag();

    // builds the second table
    htmls::HTMLStream<"<table>","</table>"> table2;
    htmls::HTMLStream<"<tr>","</tr>"> tr2;

    for (int i = 1; i < 4; i++) {
      std::string s = std::to_string(i);
      tr2 << td(s);
    }

    tr2 << htmls::gen_end_tag();
    table2 << tr2 << htmls::gen_end_tag();

    // builds the third table


    body << table1 << table2 << htmls::gen_end_tag();
    html << body << htmls::gen_end_tag();



}