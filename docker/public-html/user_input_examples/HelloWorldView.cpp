#include <cstdlib>
#include <ctime>
#include <iterator>

#include "HelloWorldView.hpp"
#include "FleroppDB.hpp"

#include <fleropp/HTMLLiterals.hpp>
#include <fleropp/QueryString.hpp>
#include <fleropp/RequestData.hpp>
#include <fleropp/PostFile.hpp>

#include <fleropp/SQLBuilder.h>

#include <soci/soci.h>
#include <soci/error.h>
#include <soci/session.h>

INIT_VIEW(HelloWorldView);

void HelloWorldView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    using namespace SQLBuilder;

    std::string dbop = request.get_query_string().get("dbOp");

    int success = -1;

    if (dbop == "i") {
        InsertModel i;
        auto rows = i.into("hello")
        .insert("name", "testRow")
        ("date", std::time(nullptr))
        .run();

        success = (rows == 1) ? 1 : 0;
    } else if (dbop == "u") {
        UpdateModel u;

        auto rows = u.set("name", "Updated!")
                        .update("hello")
                        .where("name", "testRow")
                        .run();
        success = (rows == 1) ? 1 : 0;
    } else if (dbop == "d") {
        DeleteModel d;

        auto rows = d.from("hello").where(
            SQLBuilder::column{"name", "=", "testRow"}
            || SQLBuilder::column{"name", "=", "Updated!"}
        ).run();

        success = (rows == 1) ? 1 : 0;
    }

    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
    "<head>"_h;
        "<title>Hello, World!</title>"_h;
        "<link href=\"https:\/\/cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-aFq/bzH65dt+w6FI2ooMVUpc+21e0SRygnTpmBvdBgSdnuTN7QbdgL+OapgHtvPp\" crossorigin=\"anonymous\">"_h;
        // Bootstrap
    "</head>"_h;
    "<body>"_h;
    "<div class=\"container\">"_h;

        if (success == 0) {
            "<h1>ERROR when carrying out the db operation</h1>"_h;
        }

        "<div class=\"row\"><h1>Hello, World2!</h1></div>"_h;
        "<div class=\"row\"><h1>Hello, World&mdash;{}</h1></div>"_f("from me, in a template");
        "<div class=\"row\"><h1>Hello, {}!</h1></div>"_f(request.get_query_string().get("name"));
        "<div class=\"row\"><h1>User-Agent: {}</h1></div>"_f(request.get_header("User-Agent"));
        "<div class=\"row\"><h1>Request type: {}</h1></div>"_f(request.method());
        "<div class=\"row\"><h1>Random number: {}</h1></div>"_f(rand());
        "<div class=\"row\"><h1>SQLBuilder Test</h1></div>"_h;
        "<div class=\"row\"><h2>Data in database</h2></div>"_h;

        SQLBuilder::SelectModel s;
        s.select("id", "name as ne", "date as de")
            .where("id", ">", "1")
            .from("hello");
        // Build SQL statement used to query data
        auto r = s.run();

        "<table class=\"table\">"_h;
        "<tr>"_h;
        "<th>id</th>"_h;
        "<th>name</th>"_h;
        "<th>date</th>"_h;
        "</tr>"_h;
        "<div class=\"row\"><div>Number of rows in the result: {}</div></div>"_f(r.size());
        for (const auto &re : r)
        {
            "<tr>"_h;
            "<td>{}"_f(re.at("id"));
            "<td>{}</td>"_f(re.at("ne"));
            "<td>{}</td>"_f(re.at("de"));
            "</tr>"_h;
        }
            "</table>"_h;
            "<div class='row my-2'><div><i>SQL for above:<br>{}<br>Here ? = 1</i></div></div>"_f(s.str());

            "<form action='hello.elf'>"_h;
                "<div class='row mt-2'>"_h;
                    "<div class='col-2'>"_h;
                        "<label for='dbopselect'>Select DB operation:</label>"_h;
                    "</div>"_h;
                    "<div class='col-7'>"_h;
                        "<select name='dbOp' class='form-control' id='dbopselect'>"_h;
                            "<option value='i'>Insert</option>"_h;
                            "<option value='u'>Update</option>"_h;
                            "<option value='d'>Delete</option>"_h;
                        "</select>"_h;
                    "</div>"_h;
                    "<div class='col-3'>"_h;
                        "<input type='submit' class='btn btn-primary' value='Execute'>"_h;
                    "</div>"_h;
                "</div>"_h;
            "</form> "_h;

            "<form action='hello.elf' method='post' target='out_iframe'>"_h;
                "<div class='row'>"_h;
                    "<div class='col-8'>"_h;
                        "<input type='text' class='form-control' name='some_text' value='florp'>"_h;
                    "</div>"_h;
                    "<div class='col'>"_h;
                        "<input type='submit' class='btn btn-primary'>"_h;
                    "</div>"_h;
                "</div>"_h;
            "</form>"_h;

            "<iframe name='out_iframe'></iframe>"_h;

        "</div>"_h;
    "</body>"_h;
    "</html>"_h; 
}

void HelloWorldView::post(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    auto pstring = request.get_post_text();
    "<h1>Result: {}</h1>"_f(pstring.get("some_text"));
    "<h1>Request type: {}</h1>"_f(request.method());
}