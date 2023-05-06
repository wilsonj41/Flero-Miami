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

namespace fleropp::fpm
{
  /**
   * \class ConfigParser
   * \brief Class that parses the config file for information about source files and endpoints
   */
  class ConfigParser
  {
  public:
    /**
     * \brief A list of endpoints
     */
    std::unordered_map<std::string, std::vector<CompUnit<IViewWrapper>>> endpoints;
    /**
     * \brief A map containing database connection info
     */
    std::unordered_map<std::string, std::string> database_connection_info;
    /**
     * \brief ConfigParser constructor
     *
     * \param[in] db_driver_subdir The name of the subdirectory containing the driver
     * \param[in] db_driver_prefix The prefix to be prepended to the driver name
     */
    ConfigParser(
        const std::string &db_driver_subdir = "driver",
        const std::string &db_driver_prefix = "db_driver_");

    /**
     * \brief Function for loading a shared object by the desired file name
     *
     * \param[in] filename The name under which the shared object will be organized
     */
    void load(const std::string &filename);

  private:
    std::string m_db_driver_subdir;
    std::string m_db_driver_prefix;
  };
}

#endif /* CONFIG_PARSER_HPP */