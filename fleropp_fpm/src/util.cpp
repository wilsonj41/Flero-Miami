#include "util.hpp"
#include <grp.h>
#include <pwd.h>
#include <Group.hpp>
#include <User.hpp>
#include <unistd.h>

namespace fleropp::util::permissions {
    bool change_group_name(std::string new_group) {
        // Wrapper class for group struct
        Group group_obj(new_group);
        if (setgid(group_obj.get_group_gid()) == -1) {
            return false;
        }
        return true;
    }
    bool change_group_gid(gid_t gid) {
        if (setgid(gid) == -1) {
            return false;
        }
        return true;
    }
    bool change_user_name (std::string new_user) {
        User user_obj(new_user);

        if (setuid(user_obj.get_uid()) == -1) {
            return false;
        }
        return true;
    }
    bool change_user_uid (uid_t uid) {
        if (setuid(uid) == -1) {
            return false;
        }
        return true;
    }
}