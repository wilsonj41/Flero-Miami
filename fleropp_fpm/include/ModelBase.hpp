#include <string>
#include <unordered_map>

#include <SQLBuilder.h>

namespace sqb = SQLBuilder;

class ModelBase {
    public:
        ModelBase(const std::unordered_map<std::string, std::string>& v) {
            upsert_item(v);
        }

        static std::string table_name() {
            return "";
        }
        static std::string primary_key_name() {
            return "id";
        }

        template <typename T>
        static std::unordered_map<std::string, std::string> find(const T& id) {
            sqb::SelectModel s;

            std::unordered_map<std::string, std::string> data = s.from(table_name())
                .select("*")
                .where(primaryKey, "=", sqb::to_value(id))
                .run()
                .at(0);

            return data;
        }

    private:
        static size_t update_item(const std::unordered_map<std::string, std::string>& v) {
            sqb::UpdateModel u;
            auto primary_key = primary_key_name();
            u.update(table_name());
            u.where(primary_key, v.at(primary_key));

            for (const auto& [key, val] : v) {
                if (key != primary_key) {
                    u.set(key, val);
                }
            }

            return u.run();
        }

        static size_t insert_item(const std::unordered_map<std::string, std::string>& v) {
            sqb::InsertModel i;
            i.into(table_name());

            for (const auto& [key, val] : v) {
                i.insert(key, val);
            }

            return i.run();
        }

        /**
         * Upsert: update if the specific row is in the table
         *         or insert the row otherwise.
         *
         * \param[in] v  An unordered map that represents the data row to upsert
         *               into the database
         * \return the number of rows that got affected
         */
        static size_t upsert_item(const std::unordered_map<std::string, std::string>& v) {
            auto primary_key = primary_key_name();

            if (v.find(primary_key) != v.end()) {
                auto result = find(v.at(primary_key));

                if(result.size() > 0) { // If the record is already there
                    return update_item(v);
                }
                else
                {
                    return insert_item(v);
                }
            }
        }

        template<typename T>
        static size_t delete_item(const T& id) {
            sqb::DeleteModel d;
            d.from(table_name()).where(primary_key_name(), sqb::to_value(id));

            return d.run();
        }
};
