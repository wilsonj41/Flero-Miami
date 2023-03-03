#include "CGIEnvironment.hpp"
#include "dispatch.hpp"
#include "FCGIHandler.hpp"
#include "FCGIWorker.hpp"
#include "FleroppIO.hpp"
#include "RequestData.hpp"
#include "ScopedRedirect.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>

#include "Group.hpp"
#include "fcgio.h"
#include "fcgios.h"

#include <fmt/format.h>
#include "spdlog/spdlog.h"

#include <string>

namespace fleropp::fpm {
    FCGIHandler::FCGIHandler(const std::string &unix_sock, 
                                const unsigned int backlog) {
        // TODO: Implement permissions management for UNIX domain sockets
        umask(~(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP));
       if (!change_sock_perms()) {
            spdlog::critical("Unable to change Unix Socket Permissions");
       }

        m_fd = FCGX_OpenSocket(unix_sock.c_str(), backlog);
        
        if ( m_fd == -1 ) {
            spdlog::critical("Unable to open Unix Socket '{}'", unix_sock);
            return;
        }
        FCGX_Init();
        
        spdlog::info("Initialized Unix domain FastCGI handler at {}", unix_sock);
        // Possibly drop permissions or use capabilities to create files

    }

    FCGIHandler::FCGIHandler(const unsigned int tcp_sock, 
                                const unsigned int backlog) {
        auto sock_str = ":" + std::to_string(tcp_sock);
        m_fd = FCGX_OpenSocket(sock_str.c_str(), backlog);
        FCGX_Init();
        spdlog::info("Initialized TCP FastCGI handler at {}", sock_str);
    }

    void FCGIHandler::spawn(std::size_t n_thr) {
        for (std::size_t i = 0; i < n_thr; ++i) {
            m_workers.emplace_back([this] () { 
                fleropp::fpm::concurrency::FCGIWorker worker{m_fd, m_endpoints};
                worker.accept();
            });
        }
    }

    void FCGIHandler::wait() {
        for (auto& thread : m_workers) {
            thread.join();
        }
    }

    void FCGIHandler::load_endpoints(const endpoints_map_t& endpoints_map) {
        m_endpoints = endpoints_map;
    }

    bool FCGIHandler::change_sock_perms() {
        std::string uid_str = "www-data";
        //passwd* apache_user = getpwnam(uid_str.c_str());
        Group apache_group(uid_str);

        if (setgid(apache_group.get_group_gid()) == -1) {
            return false;
        }
        return true;
    }
}