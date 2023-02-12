#include "ConfigParser.hpp"

#include <filesystem>

#include "spdlog/spdlog.h"

namespace fleropp_fpm {
    namespace pt = boost::property_tree;
    using namespace std;

    ConfigParser::ConfigParser(const string &lib_dir) : m_lib_dir(lib_dir) {}

    void ConfigParser::load(const string &filename) {
        try {
            // Create empty property tree object
            pt::ptree tree;
            // Parse the JSON into the property tree.
            pt::read_json(filename, tree);

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
                string uri = endpoint.get<string>("uri");

                vector<CompUnit<IViewInterface>> dependencies;
                for (auto &it2 : endpoint.get_child("dependencies")) {
                    pt::ptree dependency = it2.second; // stores the data of the curr idx of the dependencies array
                    string shared_object = this->m_lib_dir + "/" + dependency.get<string>("sharedObject");

                    vector<string> sources;

                    for (auto &it3 : dependency.get_child("source")) {
                        // If this source unit is a directory, loop through the contained filepaths
                        // and add them to the sources vector
                        if (filesystem::is_directory(this->m_lib_dir + "/" + it3.second.data())) {
                            for (const auto& dir_entry : 
                                    filesystem::recursive_directory_iterator(this->m_lib_dir + "/" + it3.second.data())) {
                                if (dir_entry.is_regular_file() && dir_entry.path().extension() == source_ext) {
                                    sources.push_back(dir_entry.path().string());
                                }
                            }
                        } else {
                            // Else just add this filepath to the sources vector
                            sources.push_back(this->m_lib_dir + "/" + it3.second.data());
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
                    spdlog::info("Share Object name: '{}'", shared_object);
                    spdlog::info("Source file(s): '{}'", fmt::join(sources, ", "));
                }
            }
            spdlog::info("Finished reading configuration file '{}'", filename);
        } catch (const exception &e) {
            spdlog::critical("Unable to parse configuration file '{}': {}", filename, e.what());
        }
    }
}