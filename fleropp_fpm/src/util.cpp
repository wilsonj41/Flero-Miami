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
        // Fork off parent to child
        switch (fork()) {
            case -1: return -1;
            case 0: break;
            default: exit(0);
        }
        // Selects if terminal sessions didn't error out
        if (setsid() == -1) {
            return -1;
        }
        // Fork off child to grandchild
        switch (fork()) {
            case -1: return -2;
            case 0: break;
            default: exit(0);
        }
        // Permissions for sockets to work
        umask(~(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP));
        // Close all file descirpotrs
       int fd;
       long int maxfd;
       maxfd = sysconf(_SC_OPEN_MAX);
       if (maxfd == -1)
               maxfd = 100;
       for (fd = 0; fd < maxfd; fd++)
           close(fd);

       close(STDIN_FILENO);

       // Open all standard descriptors to /dev/null
       fd = open("/dev/null", O_RDWR);
       if (fd != STDIN_FILENO)
           return -3;
       if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
           return -4;
       if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
           return -5;
        return 0;
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