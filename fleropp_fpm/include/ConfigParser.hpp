#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <unordered_map>
#include "CompUnit.hpp"
namespace pt = boost::property_tree;
using namespace std;

class ConfigParser
{
public:
    ConfigParser();
    ConfigParser(const string &lib_dir);

    void load(const string &filename);

private:
    unordered_map<string, std::vector<CompUnit>> _endpoints;
    string _lib_dir;
};
