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
            // Changing the content_type to lower case
            const std::string content_type = m_request_env->get("CONTENT_TYPE");
            std::string lower_case_content_type;
            std::transform(std::cbegin(content_type), std::cend(content_type),
                       std::back_inserter(lower_case_content_type),
                       [] (unsigned char c) { return std::tolower(c); });
            
            // Parsing the query string. If there is not one, just has the empty string.
            m_query_string.parse();

            // Dealing with the 2 main encoding types of Post method
            if(lower_case_content_type == "application/x-www-form-urlencoded"){
                std::string line;
                fppin >> line;
                m_post_text = QueryString(line);
                m_post_text.parse();
            } else if(lower_case_content_type.substr(0,19) == "multipart/form-data") {
                m_post_files = PostFile::loadPostData(fppin);
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

    std::vector<PostFile> RequestData::get_post_files() const {
        return m_post_files;
    }

    PostText RequestData::get_post_text() const {
        return m_post_text;
    }

}
