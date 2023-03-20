#include <string>
#include <unordered_map>
#include "IModel.hpp"

class ModelBase : public IModel<ModelBase> {
    public:
        std::string get_table_name() {
            return m_table_name;
        }

        std::string set_table_name(const std::string& table_name) {
            m_table_name = table_name;
        }

        std::string get_primary_key_name() {
            return m_primary_key_name;
        }

        std::string set_primary_key_name(const std::string& primary_key_name) {
            m_primary_key_name = primary_key_name;
        }
    private:
        std::string m_table_name;
        std::string m_primary_key_name;
};