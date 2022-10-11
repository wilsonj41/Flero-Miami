#ifndef FCGI_HANDLER_HPP
#define FCGI_HANDLER_HPP

#include "IView.hpp"
#include "CompUnit.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "fcgiapp.h"

namespace fleropp_fpm {
  using endpoints_map_t = std::unordered_map<std::string, std::vector<CompUnit<IView>>>;
    class FCGIHandler {
      public:
        FCGIHandler(const std::string &unix_sock, const unsigned int backlog = 512);
        FCGIHandler(const unsigned int tcp_sock, const unsigned int backlog = 512);
        void accept();
        void add_endpoint(endpoints_map_t &endpoints_map);
      
      private:
        int _fd;
        FCGX_Request _request;
        endpoints_map_t _endpoints;
    };
}

#endif /* FCGI_HANDLER_HPP */