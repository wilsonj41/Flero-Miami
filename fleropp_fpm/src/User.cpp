#include <User.hpp>
#include <string>
#include <array>
#include <pwd.h>


using user_unique_ptr = std::unique_ptr<passwd, std::function<void(passwd*)>>; 
User::User( std::string user_name ) {
    std::array<char, 100> buffer;
    struct passwd pass;
    struct passwd *result;
    int exit_code;

    exit_code = getpwnam_r(user_name.c_str(), &pass, reinterpret_cast<char*>(buffer.data()), buffer.size(), &result);

    if ( exit_code == 0  && result ) {
        user_unique_ptr p_user = user_unique_ptr(result, [](passwd* usr){usr = nullptr; delete usr;});
        m_uid = p_user->pw_uid;
        m_gid = p_user->pw_gid;
        m_user_name = std::string(p_user->pw_name);
        m_user_info = std::string(p_user->pw_gecos);
        m_home_dir = std::string(p_user->pw_dir);
        m_shell_program = std::string(p_user->pw_shell);
    }
    m_exit_code = exit_code;
}

uid_t User::get_uid() {
    return m_uid;
}

gid_t User::get_gid() {
    return m_gid;
}
std::string User::get_user_name() {
    return m_user_name;
}
std::string User::get_user_info() {
    return m_user_info;
}
std::string User::get_home_dir() {
    return m_home_dir;
}
std::string User::get_shell_program() {
    return m_shell_program;
}
int User::get_exit_code() {
    return m_exit_code;
}

bool User::is_initialized() {
    return (m_exit_code == 0);
}

