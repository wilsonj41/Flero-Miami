#include "FCGIHandler.hpp"

using namespace fleropp_fpm;

int main (int argc, char **argv) {
    // Construct a handler that will listen on TCP port 50001
    FCGIHandler handler{50001};
    // Add some endpoint mappings
    handler.add_endpoint("/hello.elf", "HelloWorldView");
    handler.add_endpoint("/time.elf", "TimeView");
    // Start accepting connections (blocking)
    handler.accept();
}
