#ifndef GROUP_HPP
#define GROUP_HPP
#include <string>
#include <vector>
#include <grp.h>
class Group {
    public:
        Group( std::string group_name );
        ~Group(){};
        /**
         * \brief Returns name of group
         * \return String group name
         */
        std::string get_group_name();
        /**
         * \brief Returns encrypted password of group
         * \return String encrypted password 
         */
        std::string get_group_pass();
        /**
         * \brief Returns gid of group
         * \return String  group gid
         */
        gid_t get_group_gid();
        /**
         * \brief Returns Vector of group memeber
         * \return Vector<string> Returns vector of strings of group member names
         */
        std::vector<std::string> get_grp_mem_names();
        /**
         * \brief Returns if the class has been initialized
         * \return Bool Has Goup been initialized
         */
        bool is_initialized();
        /**
         * \brief Returns exit code of getgrnam_r 
         * \return Int exit code of getgrnam_r
         */
        int get_exit_code();

    private:
        std::string m_group_name;
        std::string m_group_pass;
        gid_t   m_group_gid;
        std::vector<std::string> m_group_mem_names;
        int m_exit_code;
        std::vector<std::string> parse_group(char** p_group_mems);
};
#endif