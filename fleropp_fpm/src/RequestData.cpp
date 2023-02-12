#include "RequestData.hpp"

namespace fleropp_io {
    RequestData::RequestData(const std::string& query) : m_query{query} {
        m_query.parse();
    }
}