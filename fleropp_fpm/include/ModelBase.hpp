#include <string>
#include <unordered_map>
#include "IModel.hpp"

/**
 * \class ModelBase
 * \brief A base class for models
*/
class ModelBase : public IModel<ModelBase> {
    public:
        /**
         * \brief Function that gets the name of the table associated with the model
         * \return The name of the table
        */
        std::string get_table_name() {
            return m_table_name;
        }

        /**
         * \brief Function that sets the name of the table associated with the model
         * \param table_name The name to which the table's name should be set
        */
        std::string set_table_name(const std::string& table_name) {
            m_table_name = table_name;
        }

        /**
         * \brief Function that gets the name of the primary key associated with the model
         * \return The name of the primary key
        */
        std::string get_primary_key_name() {
            return m_primary_key_name;
        }

        /**
         * \brief Function that sets the name of the primary key associated with the model
         * \param[in] primary_key_name The new name of the primary key
        */
        std::string set_primary_key_name(const std::string& primary_key_name) {
            m_primary_key_name = primary_key_name;
        }
    private:
        std::string m_table_name;
        std::string m_primary_key_name;
};