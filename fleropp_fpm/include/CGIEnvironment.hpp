#ifndef CGI_ENVIRONMENT_HPP
#define CGI_ENVIRONMENT_HPP

#include <string>
#include <unordered_map>

namespace fleropp::io {
    class CGIEnvironment {
      public:
        explicit CGIEnvironment(char** envp);
        std::string get(const std::string& key) const;
      private:
        std::unordered_map<std::string, std::string> m_env_map;
    };
}

#endif /* CGI_ENVIRONMENT_HPP */