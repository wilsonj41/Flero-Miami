#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "CompUnit.hpp"
#include "IView.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

namespace fleropp_fpm
{
    class ConfigParser
    {
    public:
        std::unordered_map<std::string, std::vector<CompUnit<IViewWrapper>>> endpoints;
        ConfigParser(const std::string &lib_dir = "/var/www/html");

        void load(const std::string &filename);

    private:
        std::string m_lib_dir;
    };
}

#endif /* CONFIG_PARSER_HPP */