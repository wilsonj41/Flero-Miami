#include "RequestData.hpp"

namespace fleropp_io {
    const std::regex RequestData::header_regex{header_pattern};
    RequestData::RequestData(const std::string& type, std::stringstream& stream, const std::string& query_string_content) 
        : m_request_type{type}, m_query_string{query_string_content}, m_header{}, m_request_stream{stream} {
            m_query_string.parse();
        }

    void RequestData::parse() {
        std::string header_line;
        bool in_header = true;
        std::getline(m_request_stream, header_line, '\n');
        while(in_header && std::getline(m_request_stream,header_line,'\n')) {
            if(header_line == "\r") {
                in_header = false;
            } else {
                std::smatch matches;
                std::regex_search(header_line,matches,header_regex);
                m_header.emplace(std::make_pair(matches[1],matches[2]));
            }
        }
    }

    std::string RequestData::get_header(const std::string& key) const {
        const auto value = m_header.find(key);
        return (value != std::end(m_header)) ? value->second : "";
    };

    const std::string& RequestData::operator[](const std::string& key) const {
        return m_header.at(key);
    }

    QueryString RequestData::get_query_string() const {
        return m_query_string;
    }
}