#include "HTMLLiterals.hpp"
#include "TimeView.hpp"

#include <chrono>
#include <ctime>

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

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    "Content-type: text/html\r"_h;
    "\r"_h;
    "<html>"_h;
        "<head>"_h "<title>Uname Info</title>"_h "</head>"_h;
        "<body>"_h;
            "<h1>"_h; 
                std::cout << uname.sysname << ' ' << uname.release << ' ' << uname.version << '\n'; 
            "</h1>_h";
            "<h2> Current Date: "_h;
                std::cout << ctime(&now);
            "</h2>"_h;
        "</body>"_h;
    "</html>"_h;
}