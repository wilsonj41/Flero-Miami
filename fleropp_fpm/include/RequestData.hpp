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
    /**
     * \namespace fleropp::io
    */
    typedef class QueryString PostText;

    /**
     * \class RequestData
     * \brief Class that represents a parsed HTTP request
    */
    class RequestData {
        public:
            /**
             * \brief RequestData constructor
             * 
             * \param[in,out] request_env The environment variable container for the request
             */
            explicit RequestData(CGIEnvironment& request_env);
            
            /**
             * \brief Retrieves the value associated with a given header key
             * This function accesses the header and retrieves the value associated with the key, or
             * the empty string if no corresponding value exists.
             * 
             * \param[in] key The key to be searched
             * \return The value associated with the key, or the empty string
             */
            std::string get_header(const std::string& key) const;

            /**
             * \brief Returns a copy of the QueryString object to the user
             * 
             * \return A copy of the QueryString object
             */
            QueryString get_query_string() const;

            /**
             * \brief Returns a copy of the FormText object associated with the request
             * 
             * \return A copy of the FormText object
             */
            PostText get_post_text() const;

            /**
             * \brief Returns the HTTP request method
             * 
             * \return The HTTP request method
             */
            std::string method() const;

            /**
             * \brief Returns a vector of PostFile objects representing individual files
             * 
             * \return A vector of PostFile objects
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
