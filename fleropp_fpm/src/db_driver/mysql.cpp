#include "IDatabaseDriver.hpp"
#include "FleroppIO.hpp"

#include <soci/soci.h>
#include "soci/mysql/soci-mysql.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <string>
#include <limits>
#include <boost/lexical_cast.hpp>
#include <ctime>
#include <stdexcept>
#include <utility>

class mysql: public IDatabaseDriver {
    public:
        mysql(const std::string& username, const std::string& password,
        const std::string& db, const std::string& host)
        : con("mysql", std::string("host=") + host + " user=" + username + " password=" + password + " db=" + db) {}
    
    protected:
        size_t create_entry_impl(const std::string &query, const std::vector<std::string>& bindings) {

            std::string query_ready_to_prepare = placeholder_to_soci_format(query, bindings);

            soci::statement stat(con);
            for (const auto &v : bindings)
            {
                stat.exchange(soci::use(v));
            }
            stat.alloc();
            stat.prepare(query_ready_to_prepare);
            stat.define_and_bind();
            stat.execute(true);

            return stat.get_affected_rows();
        }

        std::vector<std::unordered_map<std::string, std::string>> read_entry_impl(
            const std::string& query, const std::vector<std::string>& columns,
            const std::vector<std::string>& bindings) {

            std::vector<std::unordered_map<std::string, std::string>> result;

            // auto replace_result = placeholder_to_soci_format(query);

            // auto placeholder_name_counter = replace_result.second;
            // std::string query_ready_to_prepare;

            // if (placeholder_name_counter != bindings.size()) {
            //     throw std::runtime_error("Number of bindings provided for read_entry does not match the number of placeholders in the SQL statement");
            // }

            std::string query_ready_to_prepare = placeholder_to_soci_format(query, bindings);

            // soci::rowset<soci::row> resultRowSet =
            //     (bindings.empty()
            //          ? (con.prepare << query_ready_to_prepare)
            //          : (con.prepare << "SELECT 1 FROM DUAL WHERE false"));
                     // https://stackoverflow.com/questions/1456106/how-to-select-an-empty-result-set
                     // If there are bindings, we feed resultRowSet with
                     // an empty set first and then assign its value later

            soci::statement stat(con);
            soci::row curRow; // Single row of result
            for (const auto &v : bindings)
            {
                stat.exchange(soci::use(v));
            }
            stat.exchange(soci::into(curRow)); // Tell where to put the output
            stat.alloc(); // Allocate necessary internal data structure
            stat.prepare(query_ready_to_prepare);
            stat.define_and_bind(); // Necessary for binding
            stat.execute(false); // false because we are using fetch later
            while (stat.fetch())
            {
                auto rowResult = std::unordered_map<std::string, std::string>();

                for (const auto& colName : columns) {
                    const auto &prop = curRow.get_properties(colName);

                    auto &curColumnResult = rowResult[colName];

                    auto type = prop.get_data_type();

                    try {
                        // Getting result based on the data type
                        if (type == soci::data_type::dt_string
                        || type == soci::data_type::dt_xml) {
                            curColumnResult = curRow.get<std::string>(colName);
                        } else if (type == soci::data_type::dt_double) {
                            curColumnResult = boost::lexical_cast<std::string>(curRow.get<double>(colName));
                        } else if (type == soci::data_type::dt_integer) {
                            curColumnResult = boost::lexical_cast<std::string>(curRow.get<int>(colName));
                        } else if (type == soci::data_type::dt_date) {
                            std::tm d = curRow.get<std::tm>(colName);
                            curColumnResult = std::asctime(&d);
                        } else if (type == soci::data_type::dt_long_long) {
                            curColumnResult = boost::lexical_cast<std::string>(curRow.get<long long>(colName));
                        } else if (type == soci::data_type::dt_unsigned_long_long) {
                            curColumnResult = boost::lexical_cast<std::string>(curRow.get<unsigned long long>(colName));
                        } else if (type == soci::data_type::dt_blob) {
                            throw std::runtime_error("Support for blob not implemented!");
                            // TODO: Get the blob object and uncomment code below

                            // const std::size_t bin_len = binary.get_len();
                            // char *buffer = new char[bin_len];
                            // binary.read_from_start(buffer, bin_len);
                            // curColumnResult = std::string(buffer);
                            // delete[] buffer;
                            // break;
                        }
                        else if (type == soci::data_type::dt_xml)
                        {
                            throw std::runtime_error("Support for xml not implemented!");
                        }
                    } catch (const soci::soci_error& e) {
                        curColumnResult = "";
                        spdlog::critical("Error when getting value for column \"" + colName + "\". Used empty value instead");
                    }
                }

                result.push_back(std::move(rowResult));
            }

            // for (auto it = resultRowSet.begin(); it != resultRowSet.end(); it++) {
            //     const soci::row &curRow = *it;

                
            // }

            return result;
        }

        size_t update_entry_impl(const std::string &query, const std::vector<std::string>& bindings) {
            std::string query_ready_to_prepare = placeholder_to_soci_format(query, bindings);

            soci::statement stat(con);
            for (const auto &v : bindings)
            {
                stat.exchange(soci::use(v));
            }
            stat.alloc();
            stat.prepare(query_ready_to_prepare);
            stat.define_and_bind();
            stat.execute(true);

            return stat.get_affected_rows();
        }

        size_t delete_entry_impl(const std::string &query, const std::vector<std::string>& bindings){
            std::string query_ready_to_prepare = placeholder_to_soci_format(query, bindings);

            soci::statement stat(con);
            for (const auto &v : bindings)
            {
                stat.exchange(soci::use(v));
            }
            stat.alloc();
            stat.prepare(query_ready_to_prepare);
            stat.define_and_bind();
            stat.execute(true);

            return stat.get_affected_rows();
        }

        // void connect(const std::string &username, const std::string &password,
        //              const std::string &db, const std::string &host = "localhost") {
        // }

    private:
        soci::session con;

        std::string placeholder_to_soci_format(const std::string& query, const std::vector<std::string>& bindings) {
            // Prepare for SQL statement preparation
            size_t placeholder_find_start = 0;
            size_t placeholder_name_counter = 0;
            size_t placeholder_index;
            
            // the query passed uses "?" as placeholders for user input.
            // We need to change that to match soci's required format 
            // (i.e. :NAME)
            std::string query_ready_to_prepare = query;

            while (placeholder_find_start < query_ready_to_prepare.length() 
            && (placeholder_index = query_ready_to_prepare.find('?', placeholder_find_start)) != std::string::npos) {
                placeholder_name_counter++;
                // We use this counter to make sure that the placeholder names are different
                // for different placeholders.
                // The reason this statement is here is that by the end of the while loop,
                // the value of this variable indicates how many bindings we should have.

                query_ready_to_prepare.replace(
                    placeholder_index, 
                    1, 
                    std::string(":v") + boost::lexical_cast<std::string>(placeholder_name_counter)
                );
                // We use :v1, :v2, etc. as placeholders as we do not
                // bind data by names so the actual names (v1, v2, etc)
                // do not matter here

                placeholder_find_start = placeholder_index + 1; // Start to find next "?"
            }

            if (placeholder_name_counter != bindings.size()) {
                throw std::runtime_error("Number of bindings provided for read_entry does not match the number of placeholders in the SQL statement");
            }

            return query_ready_to_prepare;
        }
};

INIT_DB_DRIVER(mysql);