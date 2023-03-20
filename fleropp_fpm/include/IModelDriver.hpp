#ifndef IMODELDRIVER_HPP
#define IMODELDRIVER_HPP

class IModelDriver {
    public:
        virtual bool entry_create(const std::string& query) = 0;
        virtual unordered_map<std::string, std::string> entry_read(const std::string& query) = 0;
        virtual bool entry_update(const std::string& query) = 0;
        virtual bool entry_delete(const std::string& query) = 0;
        virtual void connect(const std::string& username, const std::string& password, 
                             const std::string& db, const std::string& host="localhost") = 0;
}

#endif /* IMODELDRIVER_HPP */
