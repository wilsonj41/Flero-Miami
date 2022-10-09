#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <unordered_map>
#include "CompUnit.hpp"
#include "IView.hpp"

namespace fleropp_fpm
{
    class ConfigParser
    {
    public:
        std::unordered_map<std::string, std::vector<CompUnit<IView>>> _endpoints;
        ConfigParser(const std::string &lib_dir = "/var/www/html");

        void load(const std::string &filename);

    private:
        std::string _lib_dir;
    };
}
