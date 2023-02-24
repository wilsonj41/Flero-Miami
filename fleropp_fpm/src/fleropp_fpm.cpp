#include "FCGIHandler.hpp"
#include "ConfigParser.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/sinks/rotating_file_sink.h"

int main ([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    // Maximum log file size before rotation (5 MiB) and maximum number of log
    // files
    static constexpr auto max_log_size = 5'242'880, max_log_files = 5;
    spdlog::cfg::load_env_levels();
    auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                                "/var/log/fleropp.log",
                                max_log_size, max_log_files);
    auto primary_logger = std::make_shared<spdlog::logger>("FPM", rotating_file_sink);
    auto compiler_logger = std::make_shared<spdlog::logger>("compiler", rotating_file_sink);
    spdlog::set_default_logger(primary_logger);
    spdlog::register_logger(compiler_logger);
    spdlog::flush_on(spdlog::level::info);
    spdlog::set_pattern("%v");
    spdlog::info(R"(______ _                           
|  ___| |                _     _   
| |_  | | ___ _ __ ___ _| |_ _| |_ 
|  _| | |/ _ \ '__/ _ \_   _|_   _|
| |   | |  __/ | | (_) ||_|   |_|  
\_|   |_|\___|_|  \___/
)");
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::info("Flero++ FastCGI Process Manager (Flero++ FPM) version {}", "0.1");

    fleropp::fpm::ConfigParser config;
    config.load("/etc/fleropp/demo.json");
    // Construct a handler that will listen on TCP port 50001
    fleropp::fpm::FCGIHandler handler{"/tmp/bruh"};
    // Add some endpoint mappings
    handler.load_endpoints(config.endpoints);
    // Start accepting connections (blocking)
    handler.spawn(1);
    handler.wait();
}
