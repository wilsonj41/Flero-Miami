#ifndef REQUEST_DATA_HPP
#define REQUEST_DATA_HPP

#include <regex>
#include <string>
#include <sstream>
#include <unordered_map>

#include "CGIEnvironment.hpp"
#include "QueryString.hpp"
#include "PostFile.hpp"
#include "FleroppIO.hpp"

// fleropp IO namespace
namespace fleropp::io {
    typedef class QueryString PostText;
    class RequestData {
        public:
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
             * Returns a copy of the FormText object to the user.
             */
            PostText get_post_text() const;

            /**
             * Returns the request type.
             */
            std::string method() const;

            /**
             * Returns a vector of PostFile objects.
             * These objects represent individual files.
             */
            std::vector<PostFile> get_post_files() const;
            
        private:
            CGIEnvironment* m_request_env;
            std::string m_request_method;
            QueryString m_query_string;
            PostText m_post_text;
            std::vector<PostFile> m_post_files;
    };
    
}

#endif /* REQUEST_DATA_HPP */
