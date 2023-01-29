#include "QueryString.hpp"

namespace fleropp_io {
    const std::regex QueryString::query_re{query_pattern};
    QueryString::QueryString(const std::string& contents) : m_contents{contents} {}
    void QueryString::parse() {
        std::sregex_iterator begin{std::begin(m_contents), std::end(m_contents), query_re}, end{};
        std::transform(begin, end, std::inserter(m_qstring_map, std::end(m_qstring_map)), 
                       [] (const auto& match) {
                            return std::make_pair(match[1].str(), match[2].str());
                       });
    }
}