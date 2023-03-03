#include <pwd.h>
#include <functional>
#include <grp.h>
#include <memory>

class User {
    public:
        User( std::string user_name );
        ~User(){};
        uid_t get_uid();
        gid_t get_gid();
        std::string get_user_name();
        std::string get_user_info();
        std::string get_home_dir();
        std::string get_shell_program();
        bool is_initialized();
        int get_exit_code();
    private:
        uid_t m_uid;
        gid_t m_gid;
        std::string m_user_name;
        std::string m_user_info;
        std::string m_home_dir;
        std::string m_shell_program;
        int m_exit_code;
};
