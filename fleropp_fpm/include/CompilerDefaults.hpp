#include <unordered_map>
#include <string>
#include <vector>

namespace fleropp::fpm::compiler_defaults {
    const std::unordered_map<std::string, std::vector<std::string>> compiler_map = {
        {"g++", {"-std=c++20", "-shared", "-fPIC", "--no-gnu-unique", "-o", "@OBJECT", "@SOURCES"}},
        {"clang++", {"-std=c++20", "-shared", "-fPIC", "-fuse-ld=gold", "-Wl,--no-gnu-unique", "-o", "@OBJECT", "@SOURCES"}}
    };
}
