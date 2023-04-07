#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <string_view>

namespace fleropp::logging {
    constexpr std::string_view figlet = "______ _\n"
                                        "|  ___| |                _     _\n"   
                                        "| |_  | | ___ _ __ ___ _| |_ _| |_\n"
                                        "|  _| | |/ _ \\ '__/ _ \\_   _|_   _|\n"
                                        "| |   | |  __/ | | (_) ||_|   |_|\n"  
                                        "\\_|   |_|\\___|_|  \\___/\n";
    
    void init_logging(const std::string& logfile);
}

#endif /* LOGGING_HPP */
