#include "FCGIHandler.hpp"

#include "fcgio.h"

#include <string>

namespace fleropp_fpm {
    FCGIHandler::FCGIHandler(const std::string &unix_sock, 
                                const unsigned int backlog) {
        // TODO: Implement permissions management for UNIX domain sockets
        _fd = FCGX_OpenSocket(unix_sock.c_str(), backlog);
        FCGX_Init();
        FCGX_InitRequest(&_request, _fd, 0);
    }

    FCGIHandler::FCGIHandler(const unsigned int tcp_sock, 
                                const unsigned int backlog) {
        auto sock_str = ":" + std::to_string(tcp_sock);
        _fd = FCGX_OpenSocket(sock_str.c_str(), backlog);
        FCGX_Init();
        FCGX_InitRequest(&_request, _fd, 0);
    }
    
    void FCGIHandler::accept() {
        while (FCGX_Accept_r(&_request) >= 0) {
            std::string endpoint = FCGX_GetParam("REQUEST_URI", _request.envp);
            auto source = _endpoints.find(endpoint);
            if (source != _endpoints.end()) {
                auto page = source->second.get_instance();
                fcgi_ostream fout(_request.out);
                page->generate(fout);
            }
        }
    }

    void FCGIHandler::add_endpoint(const std::string &path,
                                    const std::string &basename) {
        _endpoints.emplace(std::piecewise_construct, 
                            std::forward_as_tuple(path),
                            std::forward_as_tuple(basename));
    }
}