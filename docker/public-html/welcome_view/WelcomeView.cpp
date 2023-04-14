#include <cstdlib>
#include <iterator>
#include "WelcomeView.hpp"

// HTML templating library.
#include <fleropp/HTMLLiterals.hpp>
#include <fleropp/HTMLStream.hpp>

// User-input parsing.
#include <fleropp/QueryString.hpp>
#include <fleropp/RequestData.hpp>
#include <fleropp/PostFile.hpp>

// Database Operations
#include <fleropp/SQLBuilder.h>

// Generate the C boiler plate for the web-page.
INIT_VIEW(WelcomeView);


namespace welcome_page_util {
    using namespace fleropp::literals;
    void navigation_bar_setup() {
        // Mostly just HTML and Bootstrap to set up pretty-looking Navigation bar.
        "<nav class='navbar navbar-expand-lg navbar-dark sticky-top bg-fleropp'>"_h;
            "<a class='navbar-brand px-3 fleropp_font' href='#'>Flero++</a>"_h;
            "<button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarSupportedContent\" aria-controls=\"navbarSupportedContent\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">"_h;
                "<span class=\"navbar-toggler-icon\"></span>"_h;
            "</button>"_h
            "<div id='navbarNav'>"_h;
                "<ul class='navbar-nav fleropp_prompt'>"_h;
                    "<li class='nav-item active'>"_h;
                        "<a class='nav-link' href='#'>Home</a>"_h;
                        "</li>"_h;
                    "<li class='nav-item'>"_h;
                        "<a class='nav-link' href='#Recompilation'>Auto-Compilation</a>"_h;
                        "</li>"_h;
                    "<li class='nav-item'>"_h;
                        "<a class='nav-link' href='#HTML'>HTML Templating Library</a>"_h;
                        "</li>"_h;
                    "<li class='nav-item'>"_h;
                        "<a class='nav-link' href='#Input'>User Input</a>"_h;
                        "</li>"_h;
                    "<li class='nav-item'>"_h;
                        "<a class='nav-link' href='#ORM'>Database Operations</a>"_h;
                        "</li>"_h;
                "</ul>"_h;
                "</div>"_h;
        "</nav>"_h;
    }

    void welcome_card() {
        // An introduction to Flero++
        "<div class='card'>"_h;
            // "<h5 class='card-header fleropp_card'>Introduction</h5>"_h;
            "<div class='card-body fleropp_prompt'>"_h;
                "<h5 class='card-title fw-bold'> Welcome to Flero++! </h5>"_h;
                "<p class='card-text'> Welcome to Flero++! We are thrilled to have you as a new user on our web framework. Flero++ is a powerful and flexible tool designed to help developers create dynamic web applications with ease. Whether you are a seasoned C++ developer or just starting out, Flero++ has everything you need to build robust and scalable applications. </p>"_h;
                "<p class='card-text'> Thank you for choosing Flero++ as your web development framework, and we look forward to seeing what you create with it. </p>"_h;
                "<p class='card-text'> <b>Note:</b> As you are reading the welcome page on your web browser, you should also refer back to the source code for maximum usage of the page. </p>"_h;
                "<footer class='blockquote-footer'> Flero++ Development Team </footer>"_h;
            "</div>"_h;
        "</div>"_h;
    }

    void redirect_welcome() {
        "Status: 303 See Other\r"_h;
        "Location: /welcome.elf#ORM\r"_h; // What if this website is not at /
        "\r"_h;
    }
}


// The method that handles the GET method.
void WelcomeView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals; // Declaring that we are using the literals library.
    using namespace fleropp::html_stream;
    using namespace welcome_page_util;

    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
            "<head>"_h;
                "<title>Welcome to Flero++</title>"_h;
                "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-aFq/bzH65dt+w6FI2ooMVUpc+21e0SRygnTpmBvdBgSdnuTN7QbdgL+OapgHtvPp\" crossorigin=\"anonymous\">"_h;
                "<link rel=\"icon\" type=\"image/png\" href='./welcome_view/img/fleropp.png' />"_h
                "<link href='./welcome_view/css/welcome.css' rel=\"stylesheet\">"_h;
                "<link rel='preconnect' href='https://fonts.googleapis.com'>"_h;
                "<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>"_h;
                "<link href='https://fonts.googleapis.com/css2?family=Roboto+Mono:ital@1&family=Roboto:ital,wght@1,300&display=swap' rel='stylesheet'>"_h;
                "<link href=\"https://fonts.googleapis.com/css2?family=Prompt:wght@300;700&display=swap\" rel=\"stylesheet\">"_h;
            "</head>"_h;
        

        "<body>"_h;
        navigation_bar_setup();
        welcome_card();
        
            // Details on Auto-Compilation
            "<div class='card' id='Recompilation'>"_h;
                "<h5 class='card-header fleropp_card'>Automatic Recompilation</h5>"_h;
                "<div class='card-body fleropp_prompt'>"_h;
                    "<p class='card-text'> One of the most important features of Flero++ is the automatic recompilation built-in, which makes it easy for users to see the results of their changes in real-time. By simply editing the source code and refreshing the web browser, users can immediately see the updated version of the web page. This feature eliminates the need for manual compilation, making web development faster and more efficient.  </p>"_h;
                    "<h5 class='card-title fw-bold'> Give it a try! </h5>"_h;
                    "<p class='card-text'> Find this example in the get method of the source code and uncommenting the next line of \"code\", save the source file, and then press refresh button on your browser. </p>"_h;
                    // "<p class='card-text'> I was just added! </p>"_h;
                "</div>"_h;
            "</div>"_h;


            // Details on the HTML Templating Libraries
            "<div class='card' id='HTML'>"_h;
                "<h5 class='card-header fleropp_card' >HTML Templating Libraries</h5>"_h;
                "<div class='card-body fleropp_prompt'>"_h;
                    "<h5 class='card-title fw-bold'> What are the HTML Templating Libraries? </h5>"_h;
                    "<p class='card-text'> HTML Literals and streams are C++ libraries included in Flero++ web framework that aim to simplify the process of writing HTML code in C++. These libraries eliminate the need for all the boilerplate code that is typically required in vanilla C++ to generate a line of HTML. By using HTML Literals and streams, developers can focus on creating great web applications without worrying about the details of C++. </p>"_h;

                    //HTML Literals explanation
                    "<h5 class='card-title fw-bold'> What is HTML Literals? </h5>"_h;
                    "<p class='card-text'> Fun fact: the entire welcome page you see on Flero++ web framework is actually made using the HTML Literals library! The HTML Literals library allows developers to write HTML code using vanilla HTML syntax, with minimal C++ boilerplate code required for each line. This makes writing HTML code in C++ as easy and intuitive as possible. </p>"_h;
                    "<p class='card-text'> In addition to simplifying the process of writing HTML code, the HTML Literals library in Flero++ web framework also allows for easy insertion of variables or small lines of code into HTML literal lines. By using HTML Literals library, developers can seamlessly integrate C++ code with HTML code, making the development process faster and more efficient. Below this line is an example of code insertion. </p>"_h;
                    "<p class='card-text'> To show an example of code insertion, here is a random number generator: {} (hint: Try refreshing the page! You will see the number change everytime.)"_f(rand());

                    //HTML Streams explanation
                    HTMLStream<"<div>","</div>"> stream_section;
                    stream_section << "<h5 class='card-title fw-bold'> What is HTML Streams? </h5>"
                                    << "<p class='card-text'> The entire HTML Streams section is made using the HTML Streams library! This library takes advantage of the insertion stream operator (<<) in C++, which is a familiar tool for seasoned C++ developers. The HTML Streams library is particularly useful for inserting into large HTML elements, such as div, body, and other similar elements. </p>"
                                    << gen_end_tag();
                    "{}"_f(stream_section.str());
                    "<br>"_h;
                    "<footer> <b> Please refer to the examples for more reference on how to use the libraries </b> </footer>"_h;
                "</div>"_h;    
            "</div>"_h;

            // Details on User Input
            "<div class='card' id='Input'>"_h;
                "<h5 class='card-header fleropp_card'>Processing and using user-input</h5>"_h;
                "<div class='card-body fleropp_prompt'>"_h;
                    "<h5 class='card-title fw-bold'> What is the Request Data class? </h5>"_h;
                    "<p class='card-text'> The Flero++ web framework also has the ability to handle user input into web pages with ease, thanks to the Request Data class. This class contains many ways to handle user input, such as query strings, form text, and file uploads. The Request Data class automatically parses this information, making it ready to use without the need for developers to understand the inner workings of HTTP protocol. </p>"_h;
                    "<p class='card-text'> By using the Request Data class, developers can create web applications that are more interactive. This class allows developers to easily collect and process user input, enabling them to create web applications that are tailored to the needs of their users. </p>"_h;
                    "<p class='card-text'> Here is an example of form text using an Iframe and a post method: </p>"_h;
            
                        //This is the example form.
                        "<form class='row form-group' action='welcome.elf' method='post' enctype='application/x-www-form-urlencoded' target='out_iframe'>"_h;
                        "<div class='col-2'>"_h
                            "<div class='input-group mb-3'>"_h;
                                "<div class='input-group-prepend'>"_h;
                                    "<span for='person' class='input-group-text'>Name</span>"_h
                                "</div>"_h
                                "<input type='text' class='form-control' id='person' name='person'>"_h;
                            "</div>"_h
                        "</div>"_h
                            "<div class='col-auto'>"_h
                                "<input class='btn btn-primary fleropp_card' type='submit' value=Post>"_h;
                           "</div>"_h
                        "</form>"_h;

                        //Iframe that shows the results of the form.
                        "<iframe class='border' name='out_iframe'></iframe>"_h;
                "</div>"_h;
            "</div>"_h;

            SQLBuilder::SelectModel sm;
            auto result = sm.select("id, username, password").from("users").run();

            /*
            The following SQL statement needs to be executed to create the table
            used by the following demo:
                CREATE TABLE `users` (
                    `id` BIGINT NOT NULL AUTO_INCREMENT,
                    `username` VARCHAR(100) NOT NULL,
                    `password` VARCHAR(200) NOT NULL,
                    PRIMARY KEY (`id`)
                );
            */

            "<div class='card' id='ORM'>"_h;
            "<h5 class='card-header fleropp_card'>Database Operations</h5>"_h;
            "<div class='card-body fleropp_prompt'>"_h;
            "<h5 class='card-title fw-bold'>Connecting to a Database</h5>"_h;
            "<p class='card-text'>Flero++ provides a handy way to connect to a database and operate on data within it.</p>"_h;
            "<p class='card-text'>All you have to do as a developer is to tell Flero++ your database connection info in the config file and the connection will be established automatically when the FPM is executed."_h;
            "</p>"_h;
            "<h5 class='card-title fw-bold'>CRUD Operations on Database</h5>"_h;
            "<p class='card-text'>SQL is the foundation of all data operations, but it is not always easy to write. Therefore, Flero++ incorporates a SQL Builder library where you can construct a SQL statement declaratively and run the statement whenever you want with just one function call.</p>"_h; 
            "<p class='card-text'>Here is an example of doing CRUD operations within Flero++ (use the drop-down menu to select operations you want to carry out): </p>"_h;
            "<form method='post' class='form-group row' action='welcome.elf' enctype='application/x-www-form-urlencoded'>"_h;
            "<div class='col-3'>"_h

            // Id
            "<div class='input-group mb-3'>"_h
            "<div class='input-group-prepand'>"_h
            "<span for='idField' class='input-group-text'>Id:</span>"_h
            "</div>"_h
            "<input type='text' id='idField' name='dataId' class='form-control'>"_h
            "</div>"_h

            // Username
            "<div class='input-group mb-3'>"_h
            "<div class='input-group-prepand'>"_h
            "<span for='username' class='input-group-text'>Username:</span>"_h
            "</div>"_h
            "<input type='text' id='username' name='username' class='form-control'>"_h
            "</div>"_h

            // Password
            "<div class='input-group mb-3'>"_h
            "<div class='input-group-prepand'>"_h
            "<span for='pass' class='input-group-text'>Password:</span>"_h
            "</div>"_h
            "<input type='password' id='pass' name='password' class='form-control'>"_h
            "</div>"_h

            // Operation
            "<div class='input-group mb-3'>"_h
            "<div class='input-group-prepend'>"_h
            "<span for='dbopselect' class='input-group-text'>Select DB operation:</span>"_h
            "</div>"_h
            "<select name='dbOp' class='form-control' id='dbopselect'>"_h
            "<option value='i'>Insert</option>"_h
            "<option value='u'>Update</option>"_h
            "<option value='d'>Delete</option>"_h
            "</select>"_h
            "</div>"_h

            // Submit button
            "<div class='col-3'>"_h;
            "<input type='submit' class='btn btn-primary fleropp_card' value='Execute'>"_h
            "</div>"_h;
            "</form> "_h;

            "<div class='row'>"_h
            "<div class='col'>"_h
            "<table class='table my-3'>"_h
            "<tr>"_h
            "<th scope='col'>ID</th>"_h
            "<th scope='col'>Username</th>"_h
            "<th scope='col'>Password</th>"_h
            "</tr>"_h;

            for(const auto& row : result) {
                "<tr>"_h;
                "<td>{}</td>"_f(row.at("id"));
                "<td>{}</td>"_f(row.at("username"));
                "<td>{}</td>"_f(row.at("password"));
                "</tr>"_h;
            }

            "</table>"_h
            "</div>"_h
            "</div>"_h
            "</div>"_h
            "</div>"_h;
            "</div>"_h
            "</body>"_h;
            "</html>"_h;
}


void WelcomeView::post(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    using namespace fleropp::io;
    using namespace welcome_page_util;
    using namespace SQLBuilder;

    QueryString res = request.get_post_text();

    if (!(res.get("dbOp").empty())) {
        std::string dbOp = res.get("dbOp");
        std::string id = res.get("dataId");
        std::string username = res.get("username");
        std::string password = res.get("password");

        if (dbOp == "i") {
            if (username.empty() || password.empty()) {
                "Content-type: text/plain\r"_h;
                "\r"_h;

                "Error: No username or password!"_h;
                return;
            }

            InsertModel i;

            auto result = 
            i.into("users")
            .insert("username", username)
                   ("password", password)
            .run();

            if (result <= 0) {
                "Content-type: text/plain\r"_h;
                "\r"_h;

                "Error: Insert operation error"_h;
                return;
            }
            else
            {
                // Finish operation. Direct back
                redirect_welcome();
                return;
            }
        } else if (dbOp == "d" || dbOp == "u") {
            // Check id existance
            if (id.empty()) {
               "Content-type: text/plain\r"_h;
                "\r"_h;

                "Error: No id"_h;
                return;
            }
            else
            {
                if (dbOp == "u" && username.empty() && password.empty()) {
                    "Content-type: text/plain\r"_h;
                    "\r"_h;

                    "Error: No username nor password. Do not know what to update"_h;
                    return;
                }

                SelectModel s;
                auto count = 
                s.select("id").from("users").where("id", id).run().size();

                if (count <= 0) {
                    "Content-type: text/plain\r"_h;
                    "\r"_h;

                    "Error: No data with specified id"_h;
                    return;
                }

                if (dbOp == "u") {
                    UpdateModel u;
                    u.update("users").where("id", id);
                    if (!(username.empty()))
                    {
                        u.set("username", username);
                    }

                    if (!(password.empty())) 
                    {
                        u.set("password", password);
                    }

                    auto result = u.run();

                    if (result <= 0) {
                        "Content-type: text/plain\r"_h;
                        "\r"_h;

                        "Error: Update operation error"_h;
                        return;
                    }
                    else
                    {
                        // Finish operation. Direct back
                        redirect_welcome();
                        return;
                    }
                } else if (dbOp == "d") {
                    DeleteModel d;
                    auto result = d.from("users").where("id", id).run();
                    if (result <= 0) {
                        "Content-type: text/plain\r"_h;
                        "\r"_h;

                        "Error: Update operation error"_h;
                        return;
                    }
                    else
                    {
                        // Finish operation. Direct back
                        redirect_welcome();
                        return;
                    }
                }
            }
        } else {
            "Content-type: text/plain\r"_h;
            "\r"_h;

            "Error: Invalid database operation"_h;
            return;
        }
        return;
    }

    "Content-type: text/html\r"_h;
    "\r"_h;
    if (!(res.get("person").empty())) {
        "<h2 class='fleropp_font'> This is the results of the form: {} </h2>"_f(res.get("person"));
    }
    else if (!(res.get("sumbit_type").empty())) {
    }
}