#include "FCGIHandler.hpp"
#include "ConfigParser.hpp"

using namespace fleropp_fpm;

int main (int argc, char **argv) {
    ConfigParser config;
    config.load("/etc/fleropp/demo.json");
    // Construct a handler that will listen on TCP port 50001
    FCGIHandler handler{50001};
    // Add some endpoint mappings
    handler.add_endpoint(config._endpoints);
    // Start accepting connections (blocking)
    handler.accept();
}
