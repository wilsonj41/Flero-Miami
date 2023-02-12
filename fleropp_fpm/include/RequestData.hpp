#ifndef REQUEST_DATA_HPP
#define REQUEST_DATA_HPP

#include "QueryString.hpp"

namespace fleropp_io {
    class RequestData {
      public:
        RequestData(const std::string& query);
        const QueryString& query() const { return m_query; }
        /* const std::string& get_header(const std::string& header) const { return m_headers[header]; } */

      private:
        QueryString m_query;
        /* Headers m_headers; */
    };
} 

#endif /* REQUEST_DATA_HPP */