#include "IDatabaseDriver.hpp"
#include "FleroppIO.hpp"

#include <soci/soci.h>
#include "soci/mysql/soci-mysql.h"
#include <iostream>
#include <string>

class mysql: public IDatabaseDriver {
    public:
        mysql(const std::string& username, const std::string& password,
        const std::string& db, const std::string& host)
        : con("mysql", std::string("host=") + host + " user=" + username + " password=" + password + " db=" + db) {}
    
    protected:
        bool create_entry_impl(const std::string &query) {
            fleropp::io::fppout << "create entry called on mysql!" << std::endl;

            return true;
        }

        std::unordered_map<std::string, std::string> read_entry_impl(const std::string &query) {
            fleropp::io::fppout << "Read called on mysql" << std::endl;

            return std::unordered_map<std::string, std::string>();
        }

        bool update_entry_impl(const std::string &query) {
            fleropp::io::fppout << "Update called on mysql" << std::endl;

            return true;
        }

        bool delete_entry_impl(const std::string &query){
            fleropp::io::fppout << "delete called on mysql" << std::endl;
            return true;
        }

        // void connect(const std::string &username, const std::string &password,
        //              const std::string &db, const std::string &host = "localhost") {
        // }

    private:
        soci::session con;
};

INIT_DB_DRIVER(mysql);