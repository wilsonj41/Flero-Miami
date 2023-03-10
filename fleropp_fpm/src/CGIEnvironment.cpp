#include "CGIEnvironment.hpp"
#include <stdexcept>

namespace fleropp::io {
    CGIEnvironment::CGIEnvironment(char** envp) {
        for(char **e = envp; *e != NULL; ++e) {
            const std::string s{*e};
            const auto i = s.find('=');
            if (i == std::string::npos) {
                throw std::runtime_error("Illegally formed environment");
            }
            m_env_map[s.substr(0, i)] = s.substr(i + 1);
        }
    }

    std::string CGIEnvironment::get(const std::string& key) const {
        const auto value = m_env_map.find(key);
        return (value != std::end(m_env_map)) ? value->second : "";
    }
}