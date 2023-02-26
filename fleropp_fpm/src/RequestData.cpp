#include "RequestData.hpp"
#include "FleroppIO.hpp"
#include "spdlog/spdlog.h"

#include <algorithm>
#include <cctype>
#include <ranges>

namespace fleropp::io {
    const std::regex RequestData::header_regex{header_pattern};
    RequestData::RequestData(CGIEnvironment& request_env) 
        : m_request_env{&request_env}, m_request_method{m_request_env->get("REQUEST_METHOD")}, 
          m_query_string{m_request_env->get("QUERY_STRING")} {
            m_query_string.parse();
            if(m_request_env->get("CONTENT_TYPE") == "application/x-www-form-urlencoded"){
                std::string line;
                fppin >> line;
                m_form_data = QueryString(line);
                m_form_data.parse();
            } else if(m_request_env->get("CONTENT_TYPE") == "multipart/form-data") {
                m_post_data = HttpPostData::loadPostData(fppin);
            }
    }

    std::string RequestData::get_header(const std::string& key) const {
        std::string cgi_format_key{"HTTP_"};
        std::transform(std::cbegin(key), std::cend(key),
                       std::back_inserter(cgi_format_key),
                       [] (unsigned char c) { return std::toupper(c); });
        std::replace(std::begin(cgi_format_key), std::end(cgi_format_key), '-', '_');
        return m_request_env->get(cgi_format_key);
    };

    std::string RequestData::method() const {
        return m_request_method;
    }

    QueryString RequestData::get_query_string() const {
        return m_query_string;
    }

    std::vector<HttpPostData> RequestData::get_post_data() const {
        return m_post_data;
    }

    FormText RequestData::get_form_text() const {
        return m_form_data;
    }

}
