#include <functional>
#include <iostream>

#include "fcgio.h"

#include <cstdio>
#include <csignal>

/**
 * This file contains some *very* WIP code to experiment with *manually* 
 * writing a FastCGI daemon process. Most of this will ultimately be
 * abstracted away once we write the framework library code.
 */

namespace {
    // closure that allows inclusion of argv in context
    std::function<void(int)> sighup_handler;

    // signal handler function (__sighandler_t)
    void signal_handler(int signal) { 
        sighup_handler(signal); 
    }

    // cleans up before restarting the process
    void cleanup(std::streambuf *in, std::streambuf *out, std::streambuf *err, FCGX_Request *rq, int sock_fd) {
        std::cin.rdbuf(in);
        std::cout.rdbuf(out);
        std::cerr.rdbuf(err);
        // TODO: Figure out how to replace the socket when the process is replaced
        FCGX_Free(rq, sock_fd);
    }
}

int main(int argc, char **argv) {
    // Open and bind a fastcgi socket, store file descriptor
    int sock = FCGX_OpenSocket(":50001", 10);

    // Request data structure
    FCGX_Request request;

    // Capture stream buffers so they can be reassigned upon cleanup
    std::streambuf *cin_streambuf = std::cin.rdbuf();
    std::streambuf *cout_streambuf = std::cout.rdbuf();
    std::streambuf *cerr_streambuf = std::cerr.rdbuf();

    // Define closure that is called upon receipt of SIGHUP
    // Cleans up the environment before process replaces itself
    sighup_handler = [&](int) {
        cleanup(cin_streambuf, cout_streambuf, cerr_streambuf, &request, sock);
        execv(argv[0], argv);
    };
    std::signal(SIGHUP, signal_handler);

    FCGX_Init();
    FCGX_InitRequest(&request, sock, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        // Reassign standard streambufs to the fcgi ones
        std::cin.rdbuf(&cin_fcgi_streambuf);
        std::cout.rdbuf(&cout_fcgi_streambuf);
        std::cerr.rdbuf(&cerr_fcgi_streambuf);

        // Generate some example content
        std::cout << "Content-type: text/html\r\n"
            << "\r\n"
            << "<html>\n"
            << "  <head>\n"
            << "    <title>Hello, World</title>\n"
            << "  </head>\n"
            << "  <body>\n"
            << "    <h1>Hello, World!, later</h1>\n"
            << "  </body>\n"
            << "</html>\n";
    }

    cleanup(cin_streambuf, cout_streambuf, cerr_streambuf, &request, sock);

    return EXIT_SUCCESS;
}
