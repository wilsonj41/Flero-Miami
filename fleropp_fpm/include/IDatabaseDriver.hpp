#ifndef IDATABASEDRIVER_HPP
#define IDATABASEDRIVER_HPP

#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>

#define INIT_DB_DRIVER(NAME)                                            \
extern "C" {                                                            \
    NAME* allocator(                                                    \
        const std::string& username, const std::string& password,       \
        const std::string& db, const std::string& host,                 \
        const std::string& port) {                                      \
            return new NAME(username, password, db, host, port);        \
    }                                                                   \
    void deleter(NAME* ptr) {                                           \
        delete ptr;                                                     \
    }                                                                   \
}

/**
 * \class IDatabaseDriver
 * \brief Interface class for all database drivers
*/
class IDatabaseDriver {
    public:
        /**
         * \brief IDatabaseDriver constructor
        */
        IDatabaseDriver(): m{} {}

        /**
         * \brief IDatabaseDriver deleter
        */
        virtual ~IDatabaseDriver() = default;

        /**
         * \brief Creates a new entry in the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for query bindings
         * 
         * \return The number of entries created in the database
        */
        virtual size_t create_entry(const std::string& query, const std::vector<std::string>& bindings = {}) final {
            size_t result;

            {
                std::unique_lock<std::mutex> l{m};
                result = create_entry_impl(query, bindings);
            }

            return result;
        }

        /**
         * \brief Function that reads entries from the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return A vector of <std::string, std::string>unordered_maps representing the read entries
        */
        virtual std::vector<std::unordered_map<std::string, std::string>> read_entry(const std::string& query,
                                                    const std::vector<std::string> bindings = {}) final {
            std::vector<std::unordered_map<std::string, std::string>> result;

            {
                std::unique_lock<std::mutex> l{m};
                result = read_entry_impl(query, bindings);
            }

            return result;
        }

        /**
         * \brief Updates existing entries in the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return The number of entries updated in the database
        */
        virtual size_t update_entry(const std::string& query, const std::vector<std::string>& bindings = {}) final {
            size_t result;

            {
                std::unique_lock<std::mutex> l{m};
                result = update_entry_impl(query, bindings);
            }

            return result;
        }

        /**
         * \brief Function to delete entries from the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return The number of entries updated in the database
        */
        virtual size_t delete_entry(const std::string& query, const std::vector<std::string>& bindings = {}) final {
            size_t result;
            {
                std::unique_lock<std::mutex> l{m};
                result = delete_entry_impl(query, bindings);
            }

            return result;
        }

        protected:

        /**
         * \brief Function to create a new entry in the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return The number of entries created in the database
        */
        virtual size_t create_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;

        /**
         * \brief Function to read an entry in the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return The vector of unordered maps containing database information
        */
        virtual std::vector<std::unordered_map<std::string, std::string>> read_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;
        
        /**
         * \brief Function to update an entry in the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return The number of entries updated in the database
        */
        virtual size_t update_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;

        /**
         * \brief Function to delete an entry in the database
         * 
         * \param[in] query The SQL query to be executed
         * \param[in] bindings An optional parameter for bindings for the query
         * 
         * \return The number representing whether the delete was successful
        */
        virtual size_t delete_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;
        // virtual void connect(const std::string& username, const std::string& password, 
        //                      const std::string& db, const std::string& host="localhost") = 0;
    private:
        std::mutex m;
};

#endif /* IDATABASEDRIVER_HPP */
