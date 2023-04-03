#include "ConfigParser.hpp"

#include <filesystem>
#include <exception>

#include "spdlog/spdlog.h"

namespace fleropp::fpm {
    namespace pt = boost::property_tree;

    ConfigParser::ConfigParser(
        const std::string &db_driver_subdir,
        const std::string &db_driver_prefix
    ) : m_db_driver_subdir{db_driver_subdir}, m_db_driver_prefix{db_driver_prefix} {}

    void ConfigParser::load(const std::string &filename) {
        try {
            // Create empty property tree object
            pt::ptree tree;
            // Parse the JSON into the property tree.
            pt::read_json(filename, tree);

            // Get the webroot (if supplied); this is where the shared objects are placed
            const auto webroot = tree.get_optional<std::filesystem::path>("webroot").value_or("/var/www/html");

            // Get database information elements
            auto arg_database = tree.get_child_optional("database");
            if(arg_database) {
                for(auto& field : arg_database.get()) {
                    spdlog::info("Database information: {}:{}", field.first.data(), field.second.data());

                    std::string key = field.first.data();
                    std::string value = field.second.data();

                    if (key == "driver") { // If this is the driver entry, we turn it into the full path for the driver DSO
                        if (!value.empty()) {
                            std::filesystem::path driver_full_path = (webroot / m_db_driver_subdir / m_db_driver_prefix).concat(value + ".so");

                            if (!std::filesystem::exists(driver_full_path)) {
                                const std::string error = "Driver file " + driver_full_path.string() + " not found";

                                spdlog::critical(error);
                            }

                            value = std::move(driver_full_path);
                        } else {
                            const std::string error = "Driver entry cannot have an empty value.";
                            spdlog::critical(error);
                        }
                    }

                    database_connection_info.emplace(key, value);
                }

                if (database_connection_info.find("driver") == database_connection_info.end()) {
                    spdlog::critical("No driver specified for database");
                }

                if (database_connection_info.find("host") == database_connection_info.end()) {
                    spdlog::warn("No host specified in database connection, using localhost");

                    database_connection_info["host"] = "localhost";
                }

                if (database_connection_info.find("port") == database_connection_info.end()) {
                    spdlog::warn("No port specified in database connection, using 3306");

                    database_connection_info["port"] = "3306";
                }
            } else {
                spdlog::info("No database information was provided.");
            }

            // Get compiler configuration elements
            boost::optional<std::string> compiler = tree.get_optional<std::string>("compiler");
            auto arg_child = tree.get_child_optional("args");

            std::vector<std::string> args;
            if (arg_child) {
                for (auto& params : arg_child.get()) {
                    args.emplace_back(params.second.data());
                }
            } 

            // Get source file extension
            auto source_ext = tree.get_optional<std::string>("sourceExtension").value_or(".cpp");

            // loop for reading through the endpoint array.
            for (auto &it1 : tree.get_child("endpoint")) {
                pt::ptree endpoint = it1.second; // stores the data of the cur idx of the endpoint array
                std::string uri = endpoint.get<std::string>("uri");

                std::vector<CompUnit<IViewWrapper>> dependencies;
                for (auto &it2 : endpoint.get_child("dependencies")) {
                    pt::ptree dependency = it2.second; // stores the data of the curr idx of the dependencies array
                    std::filesystem::path shared_object = webroot / dependency.get<std::string>("sharedObject");

                    std::vector<std::string> sources;

                    for (auto &it3 : dependency.get_child("source")) {
                        // If this source unit is a directory, loop through the contained filepaths
                        // and add them to the sources vector
                        std::filesystem::path source_path = webroot / it3.second.data();
                        if (std::filesystem::is_directory(source_path)) {
                            for (const auto& dir_entry : 
                                    std::filesystem::recursive_directory_iterator(source_path)) {
                                if (dir_entry.is_regular_file() && dir_entry.path().extension() == source_ext) {
                                    sources.push_back(dir_entry.path().string());
                                }
                            }
                        } else {
                            // Else just add this filepath to the sources vector
                            sources.push_back(source_path);
                        }
                    }

                    // creates a CompUnit object and stores it in the dependencies vector
                    if (compiler) {
                        dependencies.emplace_back(shared_object, sources, compiler.get(), args);
                    } else { 
                        dependencies.emplace_back(shared_object, sources);
                    }

                    endpoints[uri] = dependencies;
                    spdlog::info("Registered endpoint: '{}'", uri);
                    spdlog::info("Share Object name: '{}'", shared_object.string());
                    spdlog::info("Source file(s): '{}'", fmt::join(sources, ", "));
                }
            }
            spdlog::info("Finished reading configuration file '{}'", filename);
        } catch (const std::exception &e) {
            spdlog::critical("Unable to parse configuration file '{}': {}", filename, e.what());
        }
    }
}