#include "CGIEnvironment.hpp"
#include "dispatch.hpp"
#include "status_codes.hpp"
#include "FCGIHandler.hpp"
#include "FCGIWorker.hpp"
#include "FleroppIO.hpp"
#include "RequestData.hpp"
#include "ScopedRedirect.hpp"
#include "FleroppDB.hpp"

#include "fcgio.h"
#include "fcgios.h"

#include <fmt/format.h>
#include "spdlog/spdlog.h"
#include <boost/exception/diagnostic_information.hpp>

#include <exception>

namespace fleropp::fpm::concurrency {
    FCGIWorker::FCGIWorker(int sock_fd, endpoints_map_t& endpoints, 
                           std::shared_ptr<IDatabaseDriver> db_handle) : m_fd{sock_fd}, 
                                                                         m_endpoints{&endpoints},
                                                                         m_db_handle{std::move(db_handle)}{
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
            const auto script_name = env.get("SCRIPT_NAME");
            const auto source = m_endpoints->find(script_name);
            
            // This information is already logged by your HTTP server; prefer
            // that log in most cases.
            spdlog::debug("{} - {} {} {}", env.get("REMOTE_ADDR"),
                                           env.get("REQUEST_METHOD"),
                                           env.get("REQUEST_URI"),
                                           env.get("SERVER_PROTOCOL"));
            fcgi_streambuf in_buf{m_request.in}, out_buf{m_request.out};

            // Redirect the streams to the global convenience variables.
            fleropp::io::ScopedRedirect redir_in{in_buf, fleropp::io::fppin};
            fleropp::io::ScopedRedirect redir_out{out_buf, fleropp::io::fppout};

            // Copy the db_handle to the global convenience variable
            // so that models can make use of it
            // Note:
            // Using std::move on this will result in 
            // a random segmentation fault
            fleropp::db::db_handle = m_db_handle;

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
                    auto handler = dispatch::request_dispatch_funs[request_method]; 
                    try {
                        std::invoke(handler, page, fleropp::io::RequestData{env}); 
                    } catch (...) {
                        // Catch all other uncaught exceptions that may be thrown
                        // by one of the pages. This will avoid SIGABRT killing the
                        // FPM. We can't handle the exception, but we can at least
                        // alert the user of its existance.
                        
                        // Credit: https://stackoverflow.com/questions/315948/c-catching-all-exceptions
                        spdlog::critical("Unhandled exception while responding to '{}':\n{}", script_name, 
                                         boost::current_exception_diagnostic_information());
                        fleropp::util::status_response<"500", "Internal Server Error">(); 
                    }
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