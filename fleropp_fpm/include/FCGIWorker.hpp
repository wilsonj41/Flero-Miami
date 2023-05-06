#ifndef FCGI_WORKER_HPP
#define FCGI_WORKER_HPP

#include "FCGIHandler.hpp"

namespace fleropp::fpm::concurrency {
    /**
     * \class FCGIWorker
     * \brief Represents an entity responsible for individual units of work
     */
    class FCGIWorker {
      public:
        /**
         * \brief FCGIWorker constructor
         * 
         * \param[in] sock_fd The file descriptor associated with the socket
         *                opened by `FCGIHandler`
         * \param[in,out] endpoints The endpoints map owned by the `FCGIHandler`
         *                  that will be observed by this worker
         * \param[out] db_handle A shared pointer referencing the database
         */
        FCGIWorker(int sock_fd, endpoints_map_t& endpoints, std::shared_ptr<IDatabaseDriver> db_handle);

        /**
         * \brief Deleted copy constructor
         * 
         * It would not make sense to copy a worker.
         * 
         * \param[in] FCGIWorker The existing FCGIWorker to be deleted
         */
        FCGIWorker(const FCGIWorker&) = delete;

        /**
         * \brief Deleted FCGIWorker copy assignment operator
         * 
         * It would not make sense to copy a worker.
        */
        FCGIWorker& operator=(const FCGIWorker&) = delete;

        /**
         * \brief FCGIWorker destructor
         */
        ~FCGIWorker();

        /**
         * \brief Function used to start accepting requests
         */
        void accept();
      private:
        int m_fd;
        endpoints_map_t* const m_endpoints;
        FCGX_Request m_request;
        std::shared_ptr<IDatabaseDriver> m_db_handle;
    };
}

#endif /* FCGI_WORKER_HPP */