#ifndef FCGI_WORKER_HPP
#define FCGI_WORKER_HPP

#include "FCGIHandler.hpp"

namespace fleropp::fpm::concurrency {
    class FCGIWorker {
      public:
        FCGIWorker(int sock_fd, endpoints_map_t& endpoints);
        FCGIWorker(const FCGIWorker&) = delete;
        FCGIWorker& operator=(const FCGIWorker&) = delete;
        ~FCGIWorker();
        void accept();
      private:
        int m_fd;
        endpoints_map_t* const m_endpoints;
        FCGX_Request m_request;
    };
}

#endif /* FCGI_WORKER_HPP */