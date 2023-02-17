#ifndef REQUEST_DATA_HPP
#define REQUEST_DATA_HPP

#include <regex>
#include <string>
#include <sstream>
#include <unordered_map>
#include "QueryString.hpp"

// fleropp IO namespace
namespace fleropp_io {

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
             * \param[in] stream The stream that has the contents of the entire request.
             * \param[in] query_string_content The string that contains just the query string. 
             */
            RequestData(const std::string& type, std::stringstream& stream, const std::string& query_string_content);
            
            /**
             * Parses the header of the request into a key-value pair format that
             * is stored in the header.
             */
            void parse();

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
        private:
            std::string m_request_type;
            QueryString m_query_string;
            std::unordered_map<std::string,std::string> m_header;
            std::stringstream& m_request_stream;

    };
}

#endif /* REQUEST_DATA_HPP */