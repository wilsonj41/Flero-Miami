#include "QueryString.hpp"

// fleropp IO namespace
namespace fleropp::io {
    const std::regex QueryString::query_re{query_pattern};
    QueryString::QueryString(const std::string& contents) : m_contents{contents} {}
    void QueryString::parse() {
        std::sregex_iterator begin{std::begin(m_contents), std::end(m_contents), query_re}, end{};
        std::transform(begin, end, std::inserter(m_qstring_map, std::end(m_qstring_map)), 
                       [] (const auto& match) {
                            return std::make_pair(match[1].str(), url_decode(match[2].str()));
                       });
    }

    std::string QueryString::get(const std::string& key) const {
        const auto value = m_qstring_map.find(key);
        return (value != std::end(m_qstring_map)) ? value->second : "";
    };

    const std::string& QueryString::operator[](const std::string& key) const {
        return m_qstring_map.at(key);
    };

    // Based on the GNU cgicc url decoding implementation
    std::string QueryString::url_decode(const std::string& url) {
        const auto hex_to_char = [] (char first, char second) {
            int digit;
            digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
            digit *= 16;
            digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
            return static_cast<char>(digit);
        };

        std::string decoded;
        char c;
        for (auto iter = std::begin(url); iter != std::end(url); ++iter) {
            switch (*iter) {
                case '+':
                    decoded.append(1, ' ');
                    break;
                case '%':
                    if (std::distance(iter, std::end(url)) >= 2
                        && std::isxdigit(*(iter + 1))
                        && std::isxdigit(*(iter + 2))) {
                        c = *++iter;
                        decoded.append(1, hex_to_char(c, *++iter));
                    } else {
                        decoded.append(1, '%');
                    }
                    break;
                default:
                    decoded.append(1, *iter);
                    break;
            }
        }
        return decoded;
    }
}