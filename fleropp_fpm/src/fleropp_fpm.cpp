#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <csignal>

#include <fcgiapp.h>
#include <fcgio.h>
#include <dlfcn.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "SOLoader.hpp"
#include "IPage.hpp"

using namespace so_loader;

// Terrible global variables. These are just here to make testing easy and
// MUST be replaced with a better solution.
static time_t last_mtime;
static pid_t proc_pid;
static int sock_fd;

static SOLoader<IPage> loader("/var/www/html/HelloWorldPage.so");

// If the mtime in memory differs from the mtime of the source file, recompile.
bool recompile_if_stale(const std::string &path) {
    struct stat fd;
    if (stat(path.c_str(), &fd) == 0) {
        auto mtime = fd.st_mtim;

        if (mtime.tv_sec != last_mtime) {
            loader.close_lib();
            last_mtime = mtime.tv_sec;
            auto child_pid = fork();
            if (!child_pid) {
                chdir("/var/www/html");
                execlp("g++", "g++", "HelloWorldPage.cpp", "-std=c++17",
                        "-shared", "-fPIC", "-o", "HelloWorldPage.so",
                         nullptr);
            }
            waitpid(child_pid, nullptr, 0);
            loader.open_lib();
            return true;
        }
    }
    return false;
}

int main (int argc, char **argv) {
    recompile_if_stale("/var/www/html/HelloWorldPage.cpp");
    std::signal(SIGCHLD, SIG_IGN);
    int sock_fd = FCGX_OpenSocket(":50001", 10);
    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, sock_fd, 0);

    while (FCGX_Accept_r(&request) >= 0) {
        recompile_if_stale("/var/www/html/HelloWorldPage.cpp");
        auto page = loader.get_instance();
        fcgi_istream fin(request.in);
        fcgi_ostream fout(request.out), ferr(request.err);
        page->generate(fout);
                
        // fout << "Content-type: text/html\r\n"
        //     << "\r\n"
        //     << "<html>\n"
        //     << "  <head>\n"
        //     << "    <title>Hello, World</title>\n"
        //     << "  </head>\n"
        //     << "  <body>\n"
        //     << "    <h1>Hello, World!, later</h1>\n"
        //     << "  </body>\n"
        //     << "</html>\n";
        
        // char **env = request.envp;
        // while (*(++env)) {
        //     puts(*env);
        // }
    }

    loader.close_lib();
}
