#ifndef QUERY_STRING_HPP
#define QUERY_STRING_HPP

#include <regex>
#include <string>
#include <unordered_map>

// fleropp IO namespace
namespace fleropp_io {
    static constexpr auto query_pattern = R"(([\w+%]+)=([^&]*))";

    class QueryString {
      public:
        static const std::regex query_re;
        QueryString(const std::string& contents);
        void parse();
        const std::string& get(const std::string& key) const { return m_qstring_map.at(key); };
        const std::string& operator[](const std::string& key) const { return m_qstring_map.at(key); };
      private:
        std::string m_contents;
        std::unordered_map<std::string, std::string> m_qstring_map;
    };
}


#endif /* QUERY_STRING_HPP */