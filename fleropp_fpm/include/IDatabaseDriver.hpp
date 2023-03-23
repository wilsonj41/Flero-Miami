#ifndef IDATABASEDRIVER_HPP
#define IDATABASEDRIVER_HPP

#include <string>
#include <unordered_map>
#include <mutex>

#define INIT_DB_DRIVER(NAME)                                      \
extern "C" {                                                      \
    NAME* allocator(                                              \
        const std::string& username, const std::string& password, \
        const std::string& db, const std::string& host) {         \
            return new NAME(username, password, db, host);        \
    }                                                             \
    void deleter(NAME* ptr) {                                     \
        delete ptr;                                               \
    }                                                             \
}


class IDatabaseDriver {
    public:
        IDatabaseDriver(): m{} {}

        virtual bool create_entry(const std::string& query) final {
            bool result;

            {
                std::unique_lock<std::mutex> l{m};
                result = create_entry_impl(query);
            }

            return result;
        }

        virtual std::unordered_map<std::string, std::string> read_entry(const std::string& query) final {
            std::unordered_map<std::string, std::string> result;

            {
                std::unique_lock<std::mutex> l{m};
                result = read_entry_impl(query);
            }

            return result;
        }

        virtual bool update_entry(const std::string& query) final {
            bool result;

            {
                std::unique_lock<std::mutex> l{m};
                result = update_entry_impl(query);
            }

            return result;
        }

        virtual bool delete_entry(const std::string& query) final {
            bool result;
            {
                std::unique_lock<std::mutex> l{m};
                result = delete_entry_impl(query);
            }

            return result;
        }

        protected:
        virtual bool create_entry_impl(const std::string& query) = 0;
        virtual std::unordered_map<std::string, std::string> read_entry_impl(const std::string& query) = 0;
        virtual bool update_entry_impl(const std::string& query) = 0;
        virtual bool delete_entry_impl(const std::string& query) = 0;
        // virtual void connect(const std::string& username, const std::string& password, 
        //                      const std::string& db, const std::string& host="localhost") = 0;
    private:
        std::mutex m;
};

#endif /* IDATABASEDRIVER_HPP */
