#include "DeepestView.hpp"
#include "../../../FleroppIO.hpp"

#include <chrono>
#include <ctime>

#include <sys/utsname.h>

extern "C" {
    DeepestView *allocator() {
        return new DeepestView();
    }

    void deleter(DeepestView *ptr) {
        delete ptr;
    }

}

void DeepestView::generate() {
    struct utsname uname;
    ::uname(&uname);

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    fleropp_io::fppout << "Content-type: text/html\r\n"
       << "\r\n"
       << "<html>\n"
       << "  <head>\n"
       << "    <title>Joey's Test Webpage</title>\n"
       << "  </head>\n"
       << "  <body>\n"
       << "    <h4>"
       << "      <pre>" 
<< "Dog goes \"woof\"\n"
<< "Cat goes \"meow\"\n"
<< "Bird goes \"tweet\"\n"
<< "And mouse goes \"squeek\"\n"
<< "Cow goes \"moo\"\n"
<< "Frog goes \"croak\"\n"
<< "And the elephant goes \"toot\"\n"
<< "Ducks say \"quack\"\n"
<< "And fish go \"blub\"\n"
<< "And the seal goes \"ow ow ow\"\n"
<< "But there's one sound\n"
<< "That no one knows\n"
<< "What does the fox say?\n"
       << "</pre>\n"
       << "</h4>\n"
       << "  </body>\n"
       << "</html>\n";
}