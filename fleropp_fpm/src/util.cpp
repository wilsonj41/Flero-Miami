#include "util.hpp"
#include <grp.h>
#include <pwd.h>
#include <Group.hpp>
#include <User.hpp>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace fleropp::util {
    int daemonize() {
        switch (fork()) {
            case -1: return -1;
            case 0: break;
            default: exit(0);
        }
        if (setsid() == -1) {
            return -1;
        }
        switch (fork()) {
            case -1: return -1;
            case 0: break;
            default: exit(0);
        }
        umask(~(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP));

//        int maxfd = sysconf(_SC_OPEN_MAX);
//        if ( maxfd == -1)
//            maxfd = BD_MAX_CLOSE;
//        for ( int fd = 0; i < maxfd; ++i) {
//            close(fd);            
//        }
//
//        close(STDIN_FILENO);
//        fd = open("/dev/null", O_RDWR);
//        
//        if (fd != STDIN_FILENO) 
//            return -1;
//        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
//            return -1;
//        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
//            return -1; 
    }
}
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