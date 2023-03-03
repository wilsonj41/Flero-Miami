#include <string>
#include <vector>
#include <grp.h>
class Group {
    public:
        Group( std::string group_name );
        ~Group(){};
        std::string get_group_name();
        std::string get_group_pass();
        gid_t get_group_gid();
        std::vector<std::string> get_grp_mem_names();
        bool is_initialized();
        int get_exit_code();

    private:
        std::string m_group_name;
        std::string m_group_pass;
        gid_t   m_group_gid;
        std::vector<std::string> m_group_mem_names;
        int m_exit_code;
        std::vector<std::string> parse_group(char** p_group_mems);
};