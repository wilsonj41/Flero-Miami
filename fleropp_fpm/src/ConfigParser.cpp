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
            
            // Get the error template (if supplied); this a template file representing an compilation error page
            const auto error_template = tree.get_optional<std::filesystem::path>("errorTemplate").value_or("/etc/fleropp/error.template");

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
            //boost::optional<std::string> compiler = tree.get_optional<std::string>("compiler");
            //auto arg_child = tree.get_child_optional("args");

            auto arg_compiler = tree.get_child_optional("compiler");
            auto arg_compiler_name = boost::make_optional<std::string>("");
            std::vector<std::string> args;
            if (arg_compiler) {
                arg_compiler_name = arg_compiler->get_optional<std::string>("name");
                auto arg_compiler_args = arg_compiler->get_child_optional("args");
                if (arg_compiler_args) {
                    for (auto& params : arg_compiler_args.get()) {
                        args.emplace_back(params.second.data());
                    }
                }
            }

            // Get source file extension
            auto source_ext = tree.get_optional<std::string>("sourceExtension").value_or(".cpp");

            // Configure the error page source generator callback
            std::ifstream fd{error_template};
            const std::string error_page_source{std::istreambuf_iterator<char>{fd}, std::istreambuf_iterator<char>{}};
            // Yes, we do want to capture by value here.
            const auto view_error_fun = [error_page_source] (const auto& class_name, const auto& error_text, const auto& shared_object) { 
                                            return fmt::format(fmt::runtime(error_page_source), 
                                                               fmt::arg("class", class_name),
                                                               fmt::arg("error", error_text),
                                                               fmt::arg("filename", shared_object));
                                        };

            // loop for reading through the endpoint array.
            for (auto &it1 : tree.get_child("endpoints")) {
                pt::ptree endpoint = it1.second; // stores the data of the cur idx of the endpoint array
                std::string uri = endpoint.get<std::string>("path"); 

                std::vector<CompUnit<IViewWrapper>> dependencies;
                for (auto &it2 : endpoint.get_child("dependencies")) {
                    pt::ptree dependency = it2.second; // stores the data of the curr idx of the dependencies array
                    std::filesystem::path shared_object = webroot / dependency.get<std::string>("object");

                    std::vector<std::string> sources;

                    for (auto &it3 : dependency.get_child("sources")) {
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
                    auto arg_spec_compiler = endpoint.get_child_optional("compiler"); 
                    if (arg_spec_compiler) {
                        std::vector<std::string> spec_args;
                        auto arg_spec_compiler_name = arg_spec_compiler->get_optional<std::string>("name");
                        auto arg_spec_compiler_args = arg_spec_compiler->get_child_optional("args");
                        if (arg_spec_compiler_args) {
                            for (auto& params : arg_spec_compiler_args.get()) {
                                spec_args.emplace_back(params.second.data());
                            }
                        }
                        dependencies.emplace_back(shared_object, sources, view_error_fun, arg_spec_compiler_name.get(), spec_args);
                    } else if (arg_compiler) {
                        dependencies.emplace_back(shared_object, sources, view_error_fun, arg_compiler_name.get(), args);
                    } else { 
                        dependencies.emplace_back(shared_object, sources, view_error_fun);
                    }

                    endpoints[uri] = dependencies;
                    spdlog::info("Registered endpoint: '{}'", uri);
                    spdlog::info("Shared object name: '{}'", shared_object.string());
                    spdlog::info("Source file(s): '{}'", fmt::join(sources, ", "));
                }
            }
            spdlog::info("Finished reading configuration file '{}'", filename);
        } catch (const std::exception &e) {
            spdlog::critical("Unable to parse configuration file '{}': {}", filename, e.what());
        }
    }
}