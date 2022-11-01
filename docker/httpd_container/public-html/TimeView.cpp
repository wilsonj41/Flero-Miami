#include "HTMLLiterals.hpp"
#include "TimeView.hpp"

#include <chrono>
#include <ctime>
#include <cstdlib>

#include <sys/utsname.h>

extern "C" {
    TimeView *allocator() {
        return new TimeView();
    }

    void deleter(TimeView *ptr) {
        delete ptr;
    }
}

void TimeView::generate() {
    using namespace fleropp_literals;
    struct utsname uname;
    ::uname(&uname);

    std::cerr << "Inside View:" << &fleropp_io::fppout << '\n';

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Uname Info</title>"_h "</head>"_h;
        "<body>"_h;
            "<h1>{} {} {}</h1>"_f(uname.sysname, uname.release, uname.version); 
            "<h2> Current Date: {} </h2>"_f(ctime(&now));
            "<h3> Here is your first lucky number: {} </h3>"_f(rand());
            "<h3> Here is your second lucky number: {} </h3>"_f(rand());
        "</body>"_h;
    "</html>"_h;
}