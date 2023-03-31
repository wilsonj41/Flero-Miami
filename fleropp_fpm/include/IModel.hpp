#ifndef IMODEL_HPP
#define IMODEL_HPP

#include <string>
#include <unordered_map>

template <class Derived>
class IModel {
    public:
        void find(int id) {
            static_cast<Derived*>(this)->find_impl(id);
        }

        void find_impl(int id) {
            static_assert(false, "find with id is not implemented.");
        }

        void find(const std::string& query) {
            static_cast<Derived*>(this)->find_impl(query);
        }

        void find_impl(const std::string& query) {
            static_assert(false, "find with query is not implemented.")
        }

        template <typename T>
        T get(const std::string& col_name, T default_value) {
            return static_cast<Derived*>(this)->get_impl(col_name, default_value);
        }

        template<typename T>
        T get_impl(const std::string& col_name, T default_value) {
            static_assert(false, "get is not implemented.");
        }
        

        bool set(const std::string& col_name, const std::string& value) {
            return static_cast<Derived*>(this)->set_impl(col_name, value);
        }

        bool set_impl(const std::string& col_name, const std::string& value) {
            static_assert(false, "set is not implemented.");
        }

        bool save() {
            return static_cast<Derived*>(this)->save_impl();
        }

        bool save_impl() {
            static_assert(false, "save is not implemented.");
        }
        
        bool remove() {
            return static_cast<Derived*>(this)->remove_impl();
        }

        bool remove_impl() {
            static_assert(false, "remove is not implemented.");
        }

        template<typename T>
        T& IModel::operator[](const std::string& col_name) {
            return static_cast<Derived*>(this)->operator_get_impl(col_name);
        }

        template<typename T>
        T& operator_get_impl(const std::string& col_name) {
            static_assert(false, "operator [] is not implemented.");
        }

        template<typename T>
        T& IModel::operator[](const std::string& col_name, const std::string& value) {
            return static_cast<Derived*>(this)->operator_set_impl(col_name, value);
        }

        template<typename T>
        T& operator_set_impl(const std::string& col_name, const std::string& value) {
            static_assert(false, "operator [] is not implemented.");
        }
};
#endif /* IMODEL_HPP */
