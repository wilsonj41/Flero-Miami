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

void TimeView::generate(std::ostream &os) {
    struct utsname uname;
    ::uname(&uname);

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    os << "Content-type: text/html\r\n"
       << "\r\n"
       << "<html>\n"
       << "  <head>\n"
       << "    <title>Uname Info</title>\n"
       << "  </head>\n"
       << "  <body>\n"
       << "    <h1>" << uname.sysname << ' ' << uname.release << ' ' << uname.version << "</h1>\n"
       << "    <h2> Current Date: " << ctime(&now) << "</h2>\n"
       << "  </body>\n"
       << "</html>\n";
}