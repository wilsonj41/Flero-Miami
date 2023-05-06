#ifndef CGI_ENVIRONMENT_HPP
#define CGI_ENVIRONMENT_HPP

#include <string>
#include <unordered_map>

/**
 * \namespace fleropp::io
*/
namespace fleropp::io {
  /**
   * \class CGIEnvironment
   * \brief Represents the CGI environment used to communicate with a web server
  */
    class CGIEnvironment {
      public:
        /**
         * \brief CGIEnvironment constructor
         * 
         * \param[in] envp The CGI environment array
         */
        explicit CGIEnvironment(char** envp);

        /**
         * \brief Gets the value of a given CGI environment variable
         * 
         * \param[in] key the CGI environment variable identifier 
         */
        std::string get(const std::string& key) const;
      private:
        std::unordered_map<std::string, std::string> m_env_map;
    };
}

#endif /* CGI_ENVIRONMENT_HPP */