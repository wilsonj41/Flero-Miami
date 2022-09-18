#include <string>
#include <vector>
#include <iostream>

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern "C" {
    static int fleropp_fcgi_handler(request_rec *r);
    static void fleropp_fcgi_register_hooks(apr_pool_t *p);
    /* Dispatch list for API hooks */
    module AP_MODULE_DECLARE_DATA fleropp_fcgi_module = {
        STANDARD20_MODULE_STUFF, 
        NULL,                  /* create per-dir    config structures */
        NULL,                  /* merge  per-dir    config structures */
        NULL,                  /* create per-server config structures */
        NULL,                  /* merge  per-server config structures */
        NULL,                  /* table of config file commands       */
        fleropp_fcgi_register_hooks  /* register hooks                      */
    };
}

static time_t last_mtime;
static pid_t proc_pid;

bool recompile_if_stale(const std::string &path) {
    struct stat fd;
    if (stat(path.c_str(), &fd) == 0) {
        auto mtime = fd.st_mtim;

        if (mtime.tv_sec != last_mtime) {
            last_mtime = mtime.tv_sec;
            auto child_pid = fork();
            if (!child_pid) {
                chdir("/var/www/fcgi");
                execlp("g++", "g++", "hello_world_fcgi.cpp", "-std=c++17",
                        "-lfcgi++", "-lfcgi", "-o", "hello_world_fcgi",
                         nullptr);
            }
            waitpid(child_pid, nullptr, 0);
            return true;
        }
    }
    return false;
}

void restart_process() {
    if (proc_pid) {
        kill(proc_pid, SIGKILL);
        waitpid(proc_pid, nullptr, 0);
    }
    proc_pid = fork();
    if (!proc_pid) {
        chdir("/var/www/fcgi");
        execl("./hello_world_fcgi", "hello_world_fcgi", nullptr);
    }
}


static int fleropp_fcgi_handler(request_rec *r) {
    if (strcmp(r->handler, "hello_world_fcgi")) {
        return DECLINED;
    }

    if (recompile_if_stale("/var/www/fcgi/hello_world_fcgi.cpp")) {
        restart_process();
    }
    return OK;
}

static void fleropp_fcgi_register_hooks(apr_pool_t *p) {
    ap_hook_handler(fleropp_fcgi_handler, NULL, NULL, APR_HOOK_FIRST);
}



