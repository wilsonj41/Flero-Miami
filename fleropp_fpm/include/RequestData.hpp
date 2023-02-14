#ifndef REQUEST_DATA_HPP
#define REQUEST_DATA_HPP

#include <regex>
#include <string>
#include <sstream>
#include <unordered_map>
#include "QueryString.hpp"

namespace fleropp_io {

    static constexpr auto header_pattern = R"((\S*)\s*:\s*(.*))";

    class RequestData {
        public:
            static const std::regex header_regex;
            RequestData(const std::string& type, std::stringstream& stream, const std::string& query_string_content);
            void parse();
            std::string get_header(const std::string& key) const;
            const std::string& operator[](const std::string& key) const;
            QueryString get_query_string() const;
        private:
            std::string m_request_type;
            QueryString m_query_string;
            std::unordered_map<std::string,std::string> m_header;
            std::stringstream& m_request_stream;

    };
}

#endif /* REQUEST_DATA_HPP */