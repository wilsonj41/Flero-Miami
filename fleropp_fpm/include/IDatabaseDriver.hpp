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


class IDatabaseDriver {
    public:
        IDatabaseDriver(): m{} {}

        virtual size_t create_entry(const std::string& query, const std::vector<std::string>& bindings = {}) final {
            size_t result;

            {
                std::unique_lock<std::mutex> l{m};
                result = create_entry_impl(query, bindings);
            }

            return result;
        }

        virtual std::vector<std::unordered_map<std::string, std::string>> read_entry(const std::string& query,
                                                    const std::vector<std::string> bindings = {}) final {
            std::vector<std::unordered_map<std::string, std::string>> result;

            {
                std::unique_lock<std::mutex> l{m};
                result = read_entry_impl(query, bindings);
            }

            return result;
        }

        virtual size_t update_entry(const std::string& query, const std::vector<std::string>& bindings = {}) final {
            size_t result;

            {
                std::unique_lock<std::mutex> l{m};
                result = update_entry_impl(query, bindings);
            }

            return result;
        }

        virtual size_t delete_entry(const std::string& query, const std::vector<std::string>& bindings = {}) final {
            size_t result;
            {
                std::unique_lock<std::mutex> l{m};
                result = delete_entry_impl(query, bindings);
            }

            return result;
        }

        protected:
        virtual size_t create_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;
        virtual std::vector<std::unordered_map<std::string, std::string>> read_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;
        virtual size_t update_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;
        virtual size_t delete_entry_impl(const std::string& query, const std::vector<std::string>& bindings) = 0;
        // virtual void connect(const std::string& username, const std::string& password, 
        //                      const std::string& db, const std::string& host="localhost") = 0;
    private:
        std::mutex m;
};

#endif /* IDATABASEDRIVER_HPP */
