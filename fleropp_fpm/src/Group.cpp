#include <grp.h>
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <functional>
#include <Group.hpp>

using group_unique_ptr = std::unique_ptr<group, std::function<void(group*)>>;

Group::Group( std::string group_name ) {
    //using group_unique_ptr = std::unique_ptr<group, std::function<void(group*)>>
    //using group_unique_ptr = std::unique_ptr<group, void(*)(group*)>
    std::array<char, 100> buffer;
    struct group grp;
    struct group *result;
    int exit_code;

    exit_code = getgrnam_r(group_name.c_str(), &grp, reinterpret_cast<char*>(buffer.data()), buffer.size(), &result);

    if ( exit_code == 0 && result ) {
        group_unique_ptr p_grp = group_unique_ptr(result, [](group* grp_ptr){grp_ptr = nullptr; delete grp_ptr;});
        m_group_name = p_grp->gr_name;
        m_group_gid = p_grp->gr_gid;
        m_group_pass = p_grp->gr_passwd;
        m_group_mem_names = parse_group(p_grp->gr_mem);
    }
    m_exit_code = exit_code;
}

std::string Group::get_group_name() {
    return m_group_name;
}
std::string Group::get_group_pass() {
    return m_group_pass;
}
gid_t Group::get_group_gid() {
    return m_group_gid;
}
std::vector<std::string> Group::get_grp_mem_names() {
    return m_group_mem_names;
}
bool Group::is_initialized() {
    return (m_exit_code == 0);
}

int Group::get_exit_code() {
    return m_exit_code;
}

std::vector<std::string> Group::parse_group(char** p_group_mems) {
    std::vector<std::string> group_mems;
    for (int i = 0; p_group_mems[i] != NULL; ++i) {
        group_mems.emplace_back(p_group_mems[i]);
    }
    return group_mems;
}
