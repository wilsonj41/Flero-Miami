#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <string_view>

namespace fleropp::logging {
    constexpr std::string_view figlet = "______ _\n"
                                        "|  ___| |                _     _\n"   
                                        "| |_  | | ___ _ __ ___ _| |_ _| |_\n"
                                        "|  _| | |/ _ \\ '__/ _ \\_   _|_   _|\n"
                                        "| |   | |  __/ | | (_) ||_|   |_|\n"  
                                        "\\_|   |_|\\___|_|  \\___/\n";
    constexpr std::string_view error_page_source = R"(#include <fleropp/HTMLLiterals.hpp>
#include <fleropp/IView.hpp>

class {class} : public IView<{class}> {{
  public:
    {class}() = default;
    ~{class}() = default;
    void get(const fleropp::io::RequestData& request);
    void post(const fleropp::io::RequestData& request);
    void put(const fleropp::io::RequestData& request);
    void del(const fleropp::io::RequestData& request);
}};

INIT_VIEW({class})

void {class}::get(const fleropp::io::RequestData& request) {{
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<!DOCTYPE html>"_h;
    "<html>"_h;
        "<head><title>Compilation Failed - Flero++</title></head>"_h;
        "<body>"_h;
            "<h1>Uh-oh! Compilation Failed.</h1>"_h;
            "<h4>Flero++ encountered an error while attempting to compile <code>{{}}</code>.</h4>"_f(R"~({filename})~");
            "<hr>"_h;
            "<h4>Compiler Output</h4>"_h;
            "<pre>{{}}</pre>"_f(R"~({error})~");
        "</body>"_h;
    "</html>"_h;
}}

void {class}::post(const fleropp::io::RequestData& request) {{
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<!DOCTYPE html>"_h;
    "<html>"_h;
        "<head><title>Compilation Failed - Flero++</title></head>"_h;
        "<body>"_h;
            "<h1>Uh-oh! Compilation Failed.</h1>"_h;
            "<h4>Flero++ encountered an error while attempting to compile <code>{{}}</code>.</h4>"_f(R"~({filename})~");
            "<hr>"_h;
            "<h4>Compiler Output</h4>"_h;
            "<pre>{{}}</pre>"_f(R"~({error})~");
        "</body>"_h;
    "</html>"_h;
}}

void {class}::put(const fleropp::io::RequestData& request) {{
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<!DOCTYPE html>"_h;
    "<html>"_h;
        "<head><title>Compilation Failed - Flero++</title></head>"_h;
        "<body>"_h;
            "<h1>Uh-oh! Compilation Failed.</h1>"_h;
            "<h4>Flero++ encountered an error while attempting to compile <code>{{}}</code>.</h4>"_f(R"~({filename})~");
            "<hr>"_h;
            "<h4>Compiler Output</h4>"_h;
            "<pre>{{}}</pre>"_f(R"~({error})~");
        "</body>"_h;
    "</html>"_h;
}}

void {class}::del(const fleropp::io::RequestData& request) {{
    using namespace fleropp::literals;
    "Content-type: text/html\r"_h;
    "\r"_h;
    "<!DOCTYPE html>"_h;
    "<html>"_h;
        "<head><title>Compilation Failed - Flero++</title></head>"_h;
        "<body>"_h;
            "<h1>Uh-oh! Compilation Failed.</h1>"_h;
            "<h4>Flero++ encountered an error while attempting to compile <code>{{}}</code>.</h4>"_f(R"~({filename})~");
            "<hr>"_h;
            "<h4>Compiler Output</h4>"_h;
            "<pre>{{}}</pre>"_f(R"~({error})~");
        "</body>"_h;
    "</html>"_h;
}}
)";

    void init_logging(const std::string& logfile);
}

#endif /* LOGGING_HPP */
