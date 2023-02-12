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
        m_fd = FCGX_OpenSocket(unix_sock.c_str(), backlog);
        FCGX_Init();
        FCGX_InitRequest(&m_request, m_fd, 0);
        spdlog::info("Initialized Unix domain FastCGI handler at {}", unix_sock);
    }

    FCGIHandler::FCGIHandler(const unsigned int tcp_sock, 
                                const unsigned int backlog) {
        auto sock_str = ":" + std::to_string(tcp_sock);
        m_fd = FCGX_OpenSocket(sock_str.c_str(), backlog);
        FCGX_Init();
        FCGX_InitRequest(&m_request, m_fd, 0);
        spdlog::info("Initialized TCP FastCGI handler at {}", sock_str);
    }
    
    void FCGIHandler::accept() {
        spdlog::info("Accepting requests with file descriptor {}", m_fd);
        while (FCGX_Accept_r(&m_request) >= 0) {
            cgicc::FCgiIO fios{m_request};
            cgicc::Cgicc fcgi{&fios};
            auto source = m_endpoints.find(fios.getenv("SCRIPT_NAME"));
            // This information is already logged by your HTTP server; prefer
            // that log in most cases.
            spdlog::debug("{} - {} {} {}", fios.getenv("REMOTE_ADDR"),
                                          fios.getenv("REQUEST_METHOD"),
                                          fios.getenv("REQUEST_URI"),
                                          fios.getenv("SERVER_PROTOCOL"));
            if (source != m_endpoints.end()) {
                ScopedRedirect redir_out{fios, fleropp_io::fppout};
                ScopedRedirect redir_in{fios, fleropp_io::fppin};
                auto page = source->second[0].get_instance();
                std::invoke(cmap[fios.getenv("REQUEST_METHOD")], page, fios.getenv("QUERY_STRING"));
                //page->generate();
            }
        }
    }

    void FCGIHandler::load_endpoints(const endpoints_map_t& endpoints_map) {
        m_endpoints = endpoints_map;
    }
}