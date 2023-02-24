#include "CGIEnvironment.hpp"
#include "dispatch.hpp"
#include "status_codes.hpp"
#include "FCGIHandler.hpp"
#include "FCGIWorker.hpp"
#include "FleroppIO.hpp"
#include "RequestData.hpp"
#include "ScopedRedirect.hpp"

#include "fcgio.h"
#include "fcgios.h"

#include <fmt/format.h>
#include "spdlog/spdlog.h"

namespace fleropp::fpm::concurrency {
    FCGIWorker::FCGIWorker(int sock_fd, endpoints_map_t& endpoints) : m_fd{sock_fd}, 
                                                                      m_endpoints{&endpoints} {
        FCGX_InitRequest(&m_request, m_fd, 0);                                                                    
    }

    FCGIWorker::~FCGIWorker() {
        FCGX_Free(&m_request, m_fd);
        OS_LibShutdown();
    }

    void FCGIWorker::accept() {
        spdlog::info("New worker accepting requests (file descriptor {})", m_fd);
        while (FCGX_Accept_r(&m_request) >= 0) {
            fleropp::io::CGIEnvironment env{m_request.envp};
            const auto source = m_endpoints->find(env.get("SCRIPT_NAME"));
            
            // This information is already logged by your HTTP server; prefer
            // that log in most cases.
            spdlog::debug("{} - {} {} {}", env.get("REMOTE_ADDR"),
                                           env.get("REQUEST_METHOD"),
                                           env.get("REQUEST_URI"),
                                           env.get("SERVER_PROTOCOL"));
            fcgi_streambuf in_buf{m_request.in}, out_buf{m_request.out};
            std::istream in{&in_buf};
            std::ostream out{&out_buf};

            // Redirect the streams to the global convenience variables.
            fleropp::io::ScopedRedirect redir_in{in, fleropp::io::fppin};
            fleropp::io::ScopedRedirect redir_out{out, fleropp::io::fppout};

            if (source != m_endpoints->end()) {  
                // Prepare to dispatch the request by getting the request
                // method and getting an instance of the page. 
                const auto request_method = env.get("REQUEST_METHOD");
                const auto page = source->second[0].get_instance();

                // If page is `nullptr`, we abort the request.
                if (!page) {
                    fleropp::util::status_response<"500", "Internal Server Error">(); 
                    spdlog::error("Instance of page not found. Aborting request.");
                    FCGX_Finish_r(&m_request);
                    continue;
                }

                // From this point on, we assume that the page handle is good
                // Dispatch the request, logging a warning if the request
                // method was not understood (not in the dispatch map).
                try {
                    std::invoke(dispatch::request_dispatch_funs[request_method], 
                                page, fleropp::io::RequestData{env});
                } catch (const std::range_error&) {
                    spdlog::info("Invalid request method received: '{}'", request_method);
                    fleropp::util::status_response<"418", "I'm a teapot">();
                }
            } else {
                fleropp::util::status_response<"404", "Not Found">();
            }
            FCGX_Finish_r(&m_request);
        }
    }
}