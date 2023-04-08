#include "ConfigParser.hpp"
#include "FCGIHandler.hpp"
#include "logging.hpp"
#include <sys/stat.h>
#include <optional>
#include <string>
#include <thread>

#include <boost/program_options.hpp>

int main ([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    namespace opts = boost::program_options;

    bool should_daemonize = false;
    
    // Create the command line options description printout
    opts::options_description desc{"Flero++ FastCGI Process Manager - Allowed options"};
    desc.add_options()
        ("help,h", "print this help message and exit.")
        ("config,c", opts::value<std::string>()->default_value("/etc/fleropp/config.json"), "configuration file location.")
        ("daemonize,d", opts::bool_switch(&should_daemonize), "daemonize the process.")
        ("log,l", opts::value<std::string>()->default_value("/var/log/fleropp.log"), "log file location.")
        ("port,p", opts::value<unsigned int>(), "listen on a TCP socket with this port.")
        ("ipc-path,u", opts::value<std::string>(), "listen on a Unix-domain socket with this path.")
        ("workers,w", opts::value<std::size_t>()->default_value(std::thread::hardware_concurrency()), "the number of FastCGI workers to spawn.")
    ;
    // Parse the command line options
    opts::variables_map vm;
    opts::store(opts::parse_command_line(argc, argv, desc), vm);
    opts::notify(vm);

    // Print the help description.
    if (vm.count("help")) {
        std::cout << desc << '\n';
        return EXIT_SUCCESS;
    }
    
    // Enforce some constraints on the options
    if (vm.count("port") && vm.count("ipc-path")) {
        std::cerr << "Must specify only one of 'port' or 'ipc-path'.\n";
        return EXIT_FAILURE;
    } else if (!(vm.count("port") || vm.count("ipc-path"))) {
        std::cerr << "Must specify either 'port' or 'ipc-path'.\n";
        return EXIT_FAILURE;
    }

    if (should_daemonize && fleropp::util::daemonize()) {
        std::cerr << "Flero++ was unable to daemonize.\n";
        return EXIT_FAILURE;
    }

    // Set up logging
    fleropp::logging::init_logging(vm["log"].as<std::string>());
    
    // Parse our configuration file
    fleropp::fpm::ConfigParser config;
    config.load(vm["config"].as<std::string>()); 

    // Delay construction of the handler object, calling the correct
    // c-tor based on the type of socket supplied as an argument.
    // `std::optional` does no heap allocation, despite the fact that
    // it has overloads for `operator->` and `operator*`.
    std::optional<fleropp::fpm::FCGIHandler> handler;
    if (vm.count("port")) {
        handler.emplace(vm["port"].as<unsigned int>());
    } else if (vm.count("ipc-path")) {
        handler.emplace(vm["ipc-path"].as<std::string>());
    }

    // Add the endpoint mappings from the config file
    handler->load_endpoints(config.endpoints);

    // Can now read the database connection information provided
    // by the configuration file.
    std::unordered_map<std::string, std::string> database_info = config.database_connection_info;

    if (!database_info.empty()) {
        const std::string driver = database_info.at("driver");
        const std::string host = database_info.at("host");
        const std::string port = database_info.at("port");
        const std::string username = database_info.at("username");
        const std::string password = database_info.at("password");
        const std::string dbname = database_info.at("dbname");
        handler->connect_db(driver, username, password, dbname, host, port);
    }

    // Spawn a pool of workers to start accepting connections
    const auto n_workers = vm["workers"].as<std::size_t>();
    spdlog::info("Spawning a pool of {} workers.", n_workers);
    handler->spawn(n_workers);

    // Wait on those workers to join
    handler->wait();

    return EXIT_SUCCESS;
}
