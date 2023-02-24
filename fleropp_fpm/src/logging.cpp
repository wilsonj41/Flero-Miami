#include "logging.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace fleropp::logging {
    void init_logging(const std::string& logfile) {
        // Maximum log file size before rotation (5 MiB) and maximum number of log
        // files
        static constexpr auto max_log_size = 5'242'880, max_log_files = 5;
        spdlog::cfg::load_env_levels();
        auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                                    logfile, max_log_size, max_log_files);
        auto primary_logger = std::make_shared<spdlog::logger>("FPM", rotating_file_sink);
        auto compiler_logger = std::make_shared<spdlog::logger>("compiler", rotating_file_sink);
        spdlog::set_default_logger(primary_logger);
        spdlog::register_logger(compiler_logger);
        spdlog::flush_on(spdlog::level::info);
        
        spdlog::set_pattern("%v");
        spdlog::info(fleropp::logging::figlet);
        spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
        spdlog::info("Flero++ FastCGI Process Manager (Flero++ FPM) version {}", "0.1");
    }
}