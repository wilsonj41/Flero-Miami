#ifndef FCGI_HANDLER_HPP
#define FCGI_HANDLER_HPP

#include "IView.hpp"
#include "CompUnit.hpp"
#include "ConstexprMap.hpp"
#include "IDatabaseDriver.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>

#include "fcgiapp.h"

namespace fleropp::fpm {
    using endpoints_map_t = std::unordered_map<std::string, std::vector<CompUnit<IViewWrapper>>>; 
    /**
     * \class FCGIHandler
     * 
     * \brief Class that handles FCGI requests by spawning workers and loading endpoints, also facilitates database connection
    */
    class FCGIHandler {
      public:
        /**
         * \brief Constructs a new FCGIHandler to listen on a unix socket
         * 
         * \param[in] unix_socket Path to unix socket file
         * \param[in] backlog The number of incoming requests in the background
         */
        FCGIHandler(const std::string &unix_sock, const unsigned int backlog = 512);
        
        /**
         * \brief Constructs a new FCGIHandler to listen on a tcp socket
         * 
         * \param[in] tcp_sock The port number assigned to socket
         * \param[in] backlog The maximum number of incoming requests in the background
         */
        FCGIHandler(const unsigned int tcp_sock, const unsigned int backlog = 512);

        /**
         * \brief Spawns pool of worker threads
         * 
         * \param[in] n_thr The number of threads to spawn 
         */
        void spawn(std::size_t n_thr);

        /**
         * \brief Waits on the worker threads (blocking) 
         */
        void wait();

        /**
         * \brief Loads in a map of endpoints and their corresponding compilation units
         * 
         * Loads in a map of endpoints to their corresponding compilation units.
         * This will generally be created by a `fleropp::fpm::ConfigParser`.
         * 
         * \param[in] endpoints_map The map of endpoints to compilation units.
         */
        void load_endpoints(const endpoints_map_t& endpoints_map);

        /**
         * \brief Connects to a database using provided credentials
         * 
         * \param[in] driver Name of the database driver to be used
         * \param[in] username Username for authorization
         * \param[in] password Password for authorization
         * \param[in] dbname Name of the database to connect to
         * \param[in] host Hostname of database server
         * \param[in] port Port number to be used for database connection
        */
        void connect_db(
            const std::string &driver, const std::string &username,
            const std::string &password, const std::string &dbname,
            const std::string &host, const std::string &port);

      private: 
        int m_fd;
        FCGX_Request m_request;
        endpoints_map_t m_endpoints;
        std::shared_ptr<IDatabaseDriver> m_db_handle;
        std::vector<std::thread> m_workers;
        bool change_sock_perms();
    };
}

#endif /* FCGI_HANDLER_HPP */