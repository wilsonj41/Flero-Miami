#include "JavascriptExample.hpp"
#include "HTMLStream.hpp"
#include "HTMLLiterals.hpp"
#include "cgicc/HTMLClasses.h"
#include <string>


using namespace fleropp_literals;
using namespace cgicc;
using namespace std;
namespace htmls = fleropp_html_stream;

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_head_script
// Note: Inline javascript seems to only work when it is not inside of a header.

extern "C" {
    JavascriptExample *allocator() {
        return new JavascriptExample();
    }

    void deleter(JavascriptExample *ptr) {
        delete ptr;
    }

}

// Generates the Javascript logic that runs this page.
void generateJS(htmls::HTMLStream<"<script type=\"text/javascript\">", "</script>"> &script) {
    script << "function myFunction() { document.getElementById(\"demo\").innerHTML = \"Hello JavaScript!\";} \n" << htmls::gen_end_tag();
}

void JavascriptExample::generate() {
    htmls::HTMLStream<"<html>", "</html>", htmls::dump_on_end> html;
    htmls::HTMLStream<"<body>", "</body>"> body;
    htmls::HTMLStream<"<head>", "</head>"> head;

    htmls::HTMLStream<"<script type=\"text/javascript\">", "</script>"> script;
    generateJS(script);

    body << h1("My Web Page") 
        << p("A Paragraph").set("id","demo") 
        << button("Try it").set("type","button").set("onclick","myFunction()") << htmls::gen_end_tag();
    head << script << htmls::gen_end_tag();
    html << head << body << htmls::gen_end_tag();
}