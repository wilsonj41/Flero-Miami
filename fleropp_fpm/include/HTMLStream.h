#include <sstream>
#include <iostream>
#include <string>

class HTMLStream : public std::ostream {
  friend HTMLStream operator<<(HTMLStream &h1, std::string &s1);

    public:
    //Constructor
    HTMLStream(std::string start_tag, std::string end_tag) : _start_tag{start_tag}, _end_tag{end_tag} {}
    HTMLStream gen_end_tag(HTMLStream &h1);

    private:
        std::stringstream m_ss;
        std::string _start_tag;
        std::string _end_tag;
};
