#include "FCGIHandler.hpp"
#include "FleroppIO.hpp"
#include "ScopedRedirect.hpp"

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPStatusHeader.h"
#include "cgicc/HTMLClasses.h"

#include "fcgio.h"

#include "FCgiIO.hpp"

#include "spdlog/spdlog.h"

#include <string>

namespace fleropp_fpm {
    FCGIHandler::FCGIHandler(const std::string &unix_sock, 
                                const unsigned int backlog) {
        // TODO: Implement permissions management for UNIX domain sockets
        _fd = FCGX_OpenSocket(unix_sock.c_str(), backlog);
        FCGX_Init();
        FCGX_InitRequest(&_request, _fd, 0);
        spdlog::info("Initialized Unix domain FastCGI handler at {}", unix_sock);
    }

    FCGIHandler::FCGIHandler(const unsigned int tcp_sock, 
                                const unsigned int backlog) {
        auto sock_str = ":" + std::to_string(tcp_sock);
        _fd = FCGX_OpenSocket(sock_str.c_str(), backlog);
        FCGX_Init();
        FCGX_InitRequest(&_request, _fd, 0);
        spdlog::info("Initialized TCP FastCGI handler at {}", sock_str);
    }
    
    void FCGIHandler::accept() {
        spdlog::info("Accepting requests with file descriptor {}", _fd);
        while (FCGX_Accept_r(&_request) >= 0) {
            cgicc::FCgiIO fios{_request};
            cgicc::Cgicc fcgi{&fios};
            auto source = _endpoints.find(fios.getenv("SCRIPT_NAME"));
            // This information is already logged by your HTTP server; prefer
            // that log in most cases.
            spdlog::debug("{} - {} {} {}", fios.getenv("REMOTE_ADDR"),
                                          fios.getenv("REQUEST_METHOD"),
                                          fios.getenv("REQUEST_URI"),
                                          fios.getenv("SERVER_PROTOCOL"));
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