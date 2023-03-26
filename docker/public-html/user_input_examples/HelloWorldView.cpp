#include <cstdlib>
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

    // Examples derived from library repo
    // Insert
    SQLBuilder::InsertModel i;
    i.insert("score", 100)
            ("name", std::string("six"))
            ("age", (unsigned char)20)
            ("address", "beijing")
            ("create_time", nullptr)
        .into("user");

    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Hello, World!</title>"_h "</head>"_h;
        "<body>"_h;
            try {
                soci::session sql{"mysql", "host=db db=fleropp_test password=fleropp"};
                // std::vector<std::string> s{500}, k{500};
                // sql << "SELECT name, da from hello", soci::into(s), soci::into(k);

                // "<h1>DB DATA: {}</h1>"_f(s[0]);
                // "<h1>DB DATA: {}</h1>"_f(k[0]);
            }
            catch (const soci::soci_error &ex)
            {
                "<h1>DB Status: {}</h1>"_f(ex.what());
            }
            catch (const std::runtime_error &ex)
            {
                "<h1>DB Status: {}</h1>"_f(ex.what());
            }
            catch (...)
            {
                "<h1>DB Status: {}</h1>"_f("Unknown exception");
            }
            "<h1>Hello, World2!</h1>"_h;
            "<h1>Hello, World&mdash;{}</h1>"_f("from me, in a template");
            "<h1>Hello, {}!</h1>"_f(request.get_query_string().get("name"));
            "<h1>User-Agent: {}</h1>"_f(request.get_header("User-Agent"));
            "<h1>Request type: {}</h1>"_f(request.method());
            "<h1>Random number: {}</h1>"_f(rand());
            "<h1>SQLBuilder Test</h1>"_h;
            "<h2>Data in database</h2>"_h;

            SQLBuilder::SelectModel s;
           auto r = s.select("id", "name as ne", "da as de")
                .from("hello")
                .where(column({"id",">=","8"}) && (column("id") <= 10 || column("id") == 12))
                .run();
        //    InsertModel ins;
        //    ins.insert("name", "AnotherDate")("da", std::time_t(2000000)).into("hello").run();

        //    UpdateModel u;
        //    std::cout << u.update("hello").set("name", "haha").where("id", "<", "3").run() << std::endl;

        //    DeleteModel d;
        //    std::cout << d.from("hello").where( column{"id", "=", "1"} || column{"id" , "=", "2"}).run() << std::endl;

           "<table>"_h;
           "<tr><th><td>id</td><td>n</td><td>d</td></th></tr>"_h;
           "Number of rows: {}"_f(r.size());
           for (const auto& re : r)
           {
                "<tr>"_h;
                "<td>{}"_f(re.at("id"));
                "<td>{}</td>"_f(re.at("ne"));
                "<td>{}</td>"_f(re.at("de"));
                "</tr>"_h;
            }
            "</table>"_h;
            "<br>"_h;
            "<span>SQL for above:<br>{}</span>"_f(s.str());

            "<form action='hello.elf' method='post' target='out_iframe'>"_h;
            "<input type='text' name='some_text' value='florp'>"_h;
            "<input type='submit'>"_h;
            "</form>"_h;
            "<iframe name='out_iframe'></iframe>"_h;
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