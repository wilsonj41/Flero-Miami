#ifndef FCGI_HANDLER_HPP
#define FCGI_HANDLER_HPP

#include "IView.hpp"
#include "CompUnit.hpp"
#include "ConstexprMap.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "fcgiapp.h"

namespace fleropp_fpm {
  using namespace std::literals::string_view_literals;
  static constexpr std::array<std::pair<std::string_view, void (IView::*)(const fleropp_io::RequestData&)>, 2> req_vals{{{"GET"sv, &IView::generate}, {"POST"sv, &IView::generate}}};
  static constexpr auto cmap = ConstexprMap<std::string_view, void (IView::*)(const fleropp_io::RequestData&), 2>{{req_vals}};
  using endpoints_map_t = std::unordered_map<std::string, std::vector<CompUnit<IView>>>;
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