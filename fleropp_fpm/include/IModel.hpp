#ifndef IMODEL_HPP
#define IMODEL_HPP

class IModel {
    public:
        std::string table_name;
        std::string primary_key_name;

        virtual void find(int id) = 0;
        virtual void find(xonst std::string& query) = 0;
        template<typename T>
        virtual T get(const std::string& col_name, T default_value) = 0;
        virtual bool set(const std::string& col_name, const std::string& value) = 0;
        virtual bool save() = 0;
        virtual bool remove() = 0;
        template<typename T>
        virtual T& IModel::operator[](const std::string& col_name) = 0;
        template<typename T>
        virtual T& IModel::operator[](const std::string& col_name, const std::string& value) = 0;
}

#endif /* IMODEL_HPP */
