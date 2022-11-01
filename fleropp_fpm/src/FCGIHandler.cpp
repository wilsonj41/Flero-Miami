#include "FCGIHandler.hpp"
#include "FleroppIO.hpp"
#include "ScopedRedirect.hpp"

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

#include "fcgio.h"

#include "FCgiIO.hpp"

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
            cgicc::FCgiIO fios{_request};
            cgicc::Cgicc fcgi{&fios};
            auto source = _endpoints.find(fios.getenv("REQUEST_URI"));
            if (source != _endpoints.end()) {
                ScopedRedirect redir{fios, fleropp_io::fppout};
                auto page = source->second[0].get_instance();
                page->generate();
            }
        }
    }

    void FCGIHandler::add_endpoint(endpoints_map_t &endpoints_map) {
        _endpoints = endpoints_map;
    }
}