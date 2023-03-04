#ifndef FCGI_WORKER_HPP
#define FCGI_WORKER_HPP

#include "FCGIHandler.hpp"

namespace fleropp::fpm::concurrency {
    /**
     * \class Represents an entity responsible for individual untis of work
     * (requests). 
     */
    class FCGIWorker {
      public:
        /**
         * Constructor.
         * 
         * \param sock_fd The file descriptor associated with the socket
         *                opened by `FCGIHandler`.
         * \param endpoints The endpoints map owned by the `FCGIHandler`
         *                  that will be observed by this worker.
         */
        FCGIWorker(int sock_fd, endpoints_map_t& endpoints);

        // Deleted copy c-tor. It probably does not make sense to copy a worker.
        FCGIWorker(const FCGIWorker&) = delete;

        // Deleted copy assignment operator. It probably does not make sense to
        // copy a worker.
        FCGIWorker& operator=(const FCGIWorker&) = delete;

        /**
         * Destructor. 
         */
        ~FCGIWorker();

        /**
         * Start accepting requests. 
         */
        void accept();
      private:
        int m_fd;
        endpoints_map_t* const m_endpoints;
        FCGX_Request m_request;
    };
}

#endif /* FCGI_WORKER_HPP */