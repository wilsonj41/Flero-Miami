#include "JoeyView.hpp"
#include "../FleroppIO.hpp"

#include <chrono>
#include <ctime>

#include <sys/utsname.h>

extern "C" {
    JoeyView *allocator() {
        return new JoeyView();
    }

    void deleter(JoeyView *ptr) {
        delete ptr;
    }

}

void JoeyView::generate() {
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
<< "                              ........\n"
<< "                       .=-''''''''   '''--=.\n"
<< "                     :|||.==.....           ''=..\n"
<< "                    :||==-:.    ''==-..........::||:\n"
<< "                      .|'     '|.   .-''''''='''|'  .|:\n"
<< "                  ..::||   .||...|'''|'  ... '|:-||::|||.\n"
<< "                .::|'   '|:.:|''  '' |:  '' :|     '||:''||\n"
<< "               :|\"       '|:       '|:...:\"           ||\n"
<< "             :||       ..: \":-:..:        ..           '|:\n"
<< "             :|'       .:|-...       ...::'''|.          '|:\n"
<< "             ||        '|::..'''''''''''                 '|.\n"
<< "             |             '|.                          ||\n"
<< "             |            .||                           ||\n"
<< "             |:         :|'''        :.                 ||\n"
<< "             ||         ||           '|:                ||\n"
<< "             ||         |::.  ..:.   .|'                ||\n"
<< "             '|           '''''''::...:'''               ||\n"
<< "              |                                       :||:\n"
<< "            :||:                                    .::'' ||..\n"
<< "            || '|:.                              ..:''  .:|''::..\n"
<< "         .::''|. '||:.                         ..:-''  .:||'\n"
<< "      ..::|'  '||.  ''|::.               ..::-'''    .::''\n"
<< "    ::|''       '|::.  ''|:.      ..::-''''     ..:-''\n"
<< "    '             '||:..::=||: .:||||..    ..::-''\n"
<< "                    '||' :-|||''    ''|::'''\n"
<< "                     .  '|||'                      '''       ...:|\n"
       << "</pre>\n"
       << "</h4>\n"
       << "  </body>\n"
       << "</html>\n";
}