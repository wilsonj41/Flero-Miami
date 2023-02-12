#ifndef FCGI_HANDLER_HPP
#define FCGI_HANDLER_HPP

#include "IView.hpp"
#include "CompUnit.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "fcgiapp.h"

namespace fleropp_fpm {
  using endpoints_map_t = std::unordered_map<std::string, std::vector<CompUnit<IViewInterface>>>;
    class FCGIHandler {
      public:
        /***
         * Constructor
         * 
         * \param unix_socket Path to unix socket file
         * \param backlog The number of incoming requests in the background
         */
        FCGIHandler(const std::string &unix_sock, const unsigned int backlog = 512);
        /***
         * Constructor
         * 
         * \param tcp_sock Port number assigned to socket
         * \param backlog The number of incoming requests in the background
         */
        FCGIHandler(const unsigned int tcp_sock, const unsigned int backlog = 512);
        /***
         * Starts event loop and accepts incoming requests
         */
        void accept();
        void load_endpoints(const endpoints_map_t& endpoints_map);
      
      private:
        int m_fd;
        FCGX_Request m_request;
        endpoints_map_t m_endpoints;
    };
}

#endif /* FCGI_HANDLER_HPP */