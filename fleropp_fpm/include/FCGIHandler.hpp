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
        FCGIHandler(const std::string &unix_sock, const unsigned int backlog = 512);
        FCGIHandler(const unsigned int tcp_sock, const unsigned int backlog = 512);
        void accept();
        void add_endpoint(const std::string &path, const std::string &basename);
      
      private:
        int _fd;
        FCGX_Request _request;
        std::unordered_map<std::string, SOLoader<IView>> _endpoints;
    };
}

#endif /* FCGI_HANDLER_HPP */