#ifndef REQUEST_DATA_HPP
#define REQUEST_DATA_HPP

#include <regex>
#include <string>
#include <sstream>
#include <unordered_map>

#include "CGIEnvironment.hpp"
#include "QueryString.hpp"

// fleropp IO namespace
namespace fleropp::io {
    /**
     * \brief The pattern for key:value pairs in a header.
     * 
     * This will match key:value patterns found in the header of a
     * HTTP request.
     */
    static constexpr auto header_pattern = R"((\S*)\s*:\s*(.*))";

    class RequestData {
        public:
            /**
             * \brief The regular expression used to find header pairs.
             * 
             * Header fields will offten be in the form of:
             *   key : value
             */
            static const std::regex header_regex;

            /**
             * Constructor.
             * 
             * \param[in] type The type of the request. (In other words, if it is a GET, POST, PUT, or DELETE)
             * \param[in] query_string_content The string that contains just the query string. 
             */
            explicit RequestData(CGIEnvironment& request_env);
            
            /**
             * Accesses the header and retrieves the value associated with the key, or
             * the empty string if no corresponding value exists.
             * 
             * \param[in] key The key to be searched.
             */
            std::string get_header(const std::string& key) const;

            /**
             * Returns a copy of the QueryString object to the user.
             */
            QueryString get_query_string() const;

            /**
             * Returns the request type.
             */
            std::string method() const;
        private:
            CGIEnvironment* m_request_env;
            std::string m_request_method;
            QueryString m_query_string;
    };
}

#endif /* REQUEST_DATA_HPP */
