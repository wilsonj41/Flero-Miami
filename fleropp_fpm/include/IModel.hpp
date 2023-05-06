#ifndef IMODEL_HPP
#define IMODEL_HPP

#include <string>
#include <unordered_map>

/**
 * \class IModel
 * \brief Interface that defines database models
 * \tparam Derived The derived class that implements the database interactions
*/
template <class Derived>
class IModel {
    public:
        /**
         * \brief Function that finds a desired entry in the database
         * \param[in] id The ID of the entry to be found
        */
        void find(int id) {
            static_cast<Derived*>(this)->find_impl(id);
        }

        /**
         * \brief Implementation of find by ID
         * \param[in] id The ID of the entry to be found
        */
        void find_impl(int id) {
            static_assert(false, "find with id is not implemented.");
        }

        /**
         * \brief Function that finds one or more entries in the database using a query
         * \param[in] query The query to be executed
        */
        void find(const std::string& query) {
            static_cast<Derived*>(this)->find_impl(query);
        }

        /**
         * \brief Implementation of find by query
         * \param[in] The query to be executed
        */
        void find_impl(const std::string& query) {
            static_assert(false, "find with query is not implemented.")
        }

        /**
         * \brief Function that gets a value from the model based on column name
         * \tparam T The type of the value to be returned
         * \param[in] col_name The name of the column to be retrieved
         * \param[in] default_value The default value to be returned if the column doesn't exist
         * \return The value of the specified column
        */
        template <typename T>
        T get(const std::string& col_name, T default_value) {
            return static_cast<Derived*>(this)->get_impl(col_name, default_value);
        }

        /**
         * \brief Implementation of get by column name
         * \tparam T The type of the value to be returned
         * \param[in] col_name The name of the column to be retrieved
         * \param[in] default_value The default value to be returned if the column doesn't exist
         * \return The value of the specified column
        */
        template<typename T>
        T get_impl(const std::string& col_name, T default_value) {
            static_assert(false, "get is not implemented.");
        }
        
        /**
         * \brief Function that sets the value of a column in the model
         * \param[in] col_name The name of the column to set
         * \param[in] value The value to which the column should be set
         * \return True if the value was successfully set, false otherwise
        */
        bool set(const std::string& col_name, const std::string& value) {
            return static_cast<Derived*>(this)->set_impl(col_name, value);
        }

        /**
         * \brief Implementation of set method
         * \param[in] col_name The name of the column to be set
         * \param[in] value The value to which the column should be set
         * \return True if the value was successfully set, false otherwise
        */
        bool set_impl(const std::string& col_name, const std::string& value) {
            static_assert(false, "set is not implemented.");
        }

        /**
         * \brief Function that saves a model entry
         * \return True if the model entry was saved successfully, false otherwise
        */
        bool save() {
            return static_cast<Derived*>(this)->save_impl();
        }

        /**
         * \brief Implementation of the save method
         * \return True if the model was removed successfully, false otherwise
        */
        bool save_impl() {
            static_assert(false, "save is not implemented.");
        }
        
        /**
         * \brief Function that removes a model entry
         * \return True if the model was removed successfully, false otherwise
        */
        bool remove() {
            return static_cast<Derived*>(this)->remove_impl();
        }

        /**
         * \brief Implementation of the remove function
         * \return True if the model was removed successfully, false otherwise
        */
        bool remove_impl() {
            static_assert(false, "remove is not implemented.");
        }

        /**
         * \brief Function that gets a reference to the value of a model column using the [] operator
         * \tparam T The type of the value to be gotten
         * \param[in] col_name The name of the column for which the value should be gotten
         * \return A reference to the value of the column (T&)
        */
        template<typename T>
        T& IModel::operator[](const std::string& col_name) {
            return static_cast<Derived*>(this)->operator_get_impl(col_name);
        }

        /**
         * \brief Implementation of the [] operator
         * \tparam T The type of the value to be gotten
         * \param[in] col_name The name of the column for which the value should be gotten
         * \return A reference to the value of the column (T&)
        */
        template<typename T>
        T& operator_get_impl(const std::string& col_name) {
            static_assert(false, "operator [] is not implemented.");
        }

        /**
         * \brief Function that uses the operator[] to retrieve a value
         * \param[in] col_name The name of the column to be retrieved
         * \param[in] value The value corresponding to the column which is being checked
         * \return The value of type T which refers to the entry at the specified value
        */
        template<typename T>
        T& IModel::operator[](const std::string& col_name, const std::string& value) {
            return static_cast<Derived*>(this)->operator_set_impl(col_name, value);
        }

        /**
         * \brief Implementation of the operator[] that sets entries
         * \param[in] col_name The name of the column in which the user would like to set a value
         * \param[in] value The value which the user wishes to set
         * \return A reference (T&) to the entry that has just been set
        */
        template<typename T>
        T& operator_set_impl(const std::string& col_name, const std::string& value) {
            static_assert(false, "operator [] is not implemented.");
        }
};
#endif /* IMODEL_HPP */
