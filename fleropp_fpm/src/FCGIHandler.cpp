#include "CGIEnvironment.hpp"
#include "dispatch.hpp"
#include "FCGIHandler.hpp"
#include "FleroppIO.hpp"
#include "RequestData.hpp"
#include "ScopedRedirect.hpp"

#include "fcgio.h"

#include <fmt/format.h>
#include "spdlog/spdlog.h"

#include <string>

namespace fleropp::fpm {
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
            fleropp::io::CGIEnvironment env{m_request.envp};
            const auto source = m_endpoints.find(env.get("SCRIPT_NAME"));
            // This information is already logged by your HTTP server; prefer
            // that log in most cases.
            spdlog::debug("{} - {} {} {}", env.get("REMOTE_ADDR"),
                                           env.get("REQUEST_METHOD"),
                                           env.get("REQUEST_URI"),
                                           env.get("SERVER_PROTOCOL"));
            if (source != m_endpoints.end()) { 
                fcgi_streambuf in_buf{m_request.in}, out_buf{m_request.out};
                std::istream in{&in_buf};
                std::ostream out{&out_buf};

                // Redirect the streams to the global convenience variables.
                fleropp::io::ScopedRedirect redir_in{in, fleropp::io::fppin};
                fleropp::io::ScopedRedirect redir_out{out, fleropp::io::fppout};

                // Prepare to dispatch the request by getting the request
                // method and getting an instance of the page. 
                const auto request_method = env.get("REQUEST_METHOD");
                const auto page = source->second[0].get_instance();

                // If page is `nullptr`, we abort the request.
                if (!page) {
                    fleropp::io::fppout << "Status: 500 Internal Server Error\r\n"
                                           "Content-type: text/html\r\n"
                                           "Content-length: 34\r\n\r\n"
                                           "<h1>500 Internal Server Error</h1>";
                    spdlog::error("Instance of page not found. Aborting request.");
                    continue;
                }

                // From this point on, we assume that the page handle is good
                // Dispatch the request, logging a warning if the request
                // method was not understood (not in the dispatch map).
                try {
                    std::invoke(dispatch::request_dispatch_funs[request_method], 
                                page, fleropp::io::RequestData{env});
                } catch (const std::range_error&) {
                    spdlog::warn("Invalid request method received: '{}'", request_method);
                    fleropp::io::fppout << "Status: 418 I'm a teapot\r\n"
                                           "Content-type: text/html\r\n"
                                           "Content-length: 25\r\n\r\n"
                                           "<h1>418 I'm a teapot</h1>";
                }
            }
        }
    }

    void FCGIHandler::load_endpoints(const endpoints_map_t& endpoints_map) {
        m_endpoints = endpoints_map;
    }
}