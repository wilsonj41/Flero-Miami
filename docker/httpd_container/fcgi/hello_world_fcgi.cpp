#include <functional>
#include <iostream>

#include "fcgio.h"

#include <cstdio>
#include <csignal>

namespace {
    std::function<void(int)> sighup_handler;
    void signal_handler(int signal) { 
        sighup_handler(signal); 
    }

    void cleanup(std::streambuf *in, std::streambuf *out, std::streambuf *err, FCGX_Request *rq, int sock_fd) {
        std::cin.rdbuf(in);
        std::cout.rdbuf(out);
        std::cerr.rdbuf(err);
        FCGX_Free(rq, sock_fd);
    }
}

int main(int argc, char **argv) {
    int sock = FCGX_OpenSocket(":50001", 10);
    FCGX_Request request;

    std::streambuf *cin_streambuf = std::cin.rdbuf();
    std::streambuf *cout_streambuf = std::cout.rdbuf();
    std::streambuf *cerr_streambuf = std::cerr.rdbuf();

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

        std::cin.rdbuf(&cin_fcgi_streambuf);
        std::cout.rdbuf(&cout_fcgi_streambuf);
        std::cerr.rdbuf(&cerr_fcgi_streambuf);

        std::cout << "Content-type: text/html\r\n"
            << "\r\n"
            << "<html>\n"
            << "  <head>\n"
            << "    <title>Hello, World</title>\n"
            << "  </head>\n"
            << "  <body>\n"
            << "    <h1>Hello, World!, now</h1>\n"
            << "  </body>\n"
            << "</html>\n";
    }

    cleanup(cin_streambuf, cout_streambuf, cerr_streambuf, &request, sock);

    return EXIT_SUCCESS;
}
