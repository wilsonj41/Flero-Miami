#include "dispatch.hpp"
#include "FCGIHandler.hpp"
#include "FleroppIO.hpp"
#include "ScopedRedirect.hpp"

#include "boost/lexical_cast.hpp"

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
                // Get the content length so that we can build the input
                // buffer
                int content_length;
                try {
                    content_length = boost::lexical_cast<decltype(content_length)>(fios.getenv("CONTENT_LENGTH"));
                } catch (const boost::bad_lexical_cast&) {
                    content_length = 0;
                }

                // At some point in the future, implementing a maximum POST
                // length would be smart.
                std::vector<char> buf;
                buf.reserve(content_length);
                fcgi_streambuf in_buf{m_request.in, std::data(buf), content_length};
                std::istream in{&in_buf};

                // Redirect the streams to the global convenience variables.
                ScopedRedirect redir_out{fios, fleropp_io::fppout};
                ScopedRedirect redir_in{in, fleropp_io::fppin};

                // Prepare to dispatch the request by getting the request
                // method and getting an instance of the page. 
                const auto request_method = fios.getenv("REQUEST_METHOD");
                auto page = source->second[0].get_instance();

                // Dispatch the request, logging a warning if the request
                // method was not understood (not in the dispatch map).
                try {
                    std::invoke(dispatch::request_dispatch_funs[request_method], 
                                page, fios.getenv("QUERY_STRING"));
                } catch (const std::range_error&) {
                    spdlog::warn("Invalid request method received: '{}'", request_method);
                }
            }
        }
    }

    void FCGIHandler::load_endpoints(const endpoints_map_t& endpoints_map) {
        m_endpoints = endpoints_map;
    }
}