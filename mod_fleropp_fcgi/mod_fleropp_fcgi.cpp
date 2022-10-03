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

// Use C linkage for components that will be linked with C functions
// (i.e., don't confuse C with C++ name mangling).
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

// Terrible global variables. These are just here to make testing easy and
// MUST be replaced with a better solution.
static time_t last_mtime;
static pid_t proc_pid;

// If the mtime in memory differs from the mtime of the source file, recompile.
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
                        "-lfcgi++", "-lfcgi", "-o", "hello_world_fcgi.elf",
                         nullptr);
            }
            waitpid(child_pid, nullptr, 0);
            return true;
        }
    }
    return false;
}

// Restart the FCGI daemon process
// Most of this should probably be offloaded to the process itself.
// In the commented section, we have a WIP version that sends SIGHUP
// to the process, which is assumed to have a handler that can be used
// to replace itself. The current uncommented version sends SIGTERM
// to "brute force" restarting the process. 
void restart_process() {
    // if (proc_pid) {
    //     kill(proc_pid, SIGHUP);
    //     waitpid(proc_pid, nullptr, 0);
    // } else {
    //     proc_pid = fork();
    //     if (!proc_pid) {
    //         chdir("/var/www/fcgi");
    //         execl("./hello_world_fcgi.elf", "./hello_world_fcgi.elf", nullptr);
    //     }
    // }

    if (proc_pid) {
        kill(proc_pid, SIGTERM);
        waitpid(proc_pid, nullptr, 0);
    } 
    // Doing it this way, we may have to fork twice to avoid zombie processes?
    proc_pid = fork();
    if (!proc_pid) {
        chdir("/var/www/fcgi");
        execl("./hello_world_fcgi.elf", "./hello_world_fcgi.elf", nullptr);
    }
}

// Apache handler function (forward declaration above)
static int fleropp_fcgi_handler(request_rec *r) {
    // We are only concerned with a simple case here: one endpoint.
    // See apache.conf for how a ProxyPass is used for FastCGI at
    // this same endpoint.
    if (strcmp(r->parsed_uri.path, "/hello_world_fcgi")) {
        return DECLINED;
    }

    if (recompile_if_stale("/var/www/fcgi/hello_world_fcgi.cpp")) {
        restart_process();
    }
    return OK;
}

static void fleropp_fcgi_register_hooks(apr_pool_t *p) {
    // post_read_request allows us to *look* at the request; we don't need to
    // modify it.
    ap_hook_post_read_request(fleropp_fcgi_handler, nullptr, nullptr, APR_HOOK_FIRST);
}
