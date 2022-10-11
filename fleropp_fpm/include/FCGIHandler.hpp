#ifndef FCGI_HANDLER_HPP
#define FCGI_HANDLER_HPP

#include <string>
#include <unordered_map>

#include "fcgiapp.h"

#include "IView.hpp"
#include "SOLoader.hpp"

namespace fleropp_fpm {
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

        /***
         * Maps an endpoint to a Compilation Unit
         * \param path Endpoint path 
         * \param basename Basename of Compilation Unit
         */ 
        void add_endpoint(const std::string &path, const std::string &basename);
      
      private:
        int _fd;
        FCGX_Request _request;
        std::unordered_map<std::string, SOLoader<IView>> _endpoints;
    };
}

#endif /* FCGI_HANDLER_HPP */