#include "FCGIHandler.hpp"
#include <thread>
#include <vector>
using namespace fleropp_fpm;

void handler_job1 (const unsigned int port, const std::string endpoint, const std::string basename) {
    // Construct a handler that will listen on TCP port 50001
    FCGIHandler handler{port};
    // Add some endpoint mappings
    handler.add_endpoint(endpoint, basename);
    // Start accepting connections (blocking)
    handler.accept();
}


int main (int argc, char **argv) {
    auto t1 = std::thread(handler_job1, 50001, "/hello.elf", "HelloWorldView");
    auto t2 = std::thread(handler_job1, 50002, "/hello.elf", "HelloWorldView");
    auto t3 = std::thread(handler_job1, 50003, "/hello.elf", "HelloWorldView");
    auto t4 = std::thread(handler_job1, 50004, "/hello.elf", "HelloWorldView");
    auto t5 = std::thread(handler_job1, 50005, "/hello.elf", "HelloWorldView");
    std::vector<std::thread> vecThread; 
    vecThread.push_back(std::move(t1));
    vecThread.push_back(std::move(t2));
    vecThread.push_back(std::move(t3));
    vecThread.push_back(std::move(t4));
    vecThread.push_back(std::move(t5));
    for (auto& thread : vecThread) {
        thread.join();
    }
}