#ifndef USER_HPP
#define USER_HPP

#include <pwd.h>
#include <functional>
#include <grp.h>
#include <memory>

/**
 * \class User
 * \brief A class that represents a user
*/
class User {
    public:
        /**
         * \brief Constructor that makes a User object from a given user name
         * \param[in] user_name The name of the user to be created
        */
        User( std::string user_name );

        /**
         * \brief Destructor that deletes a User object
        */
        ~User(){};

        /**
         * \brief Returns uid of User
         * \return Uid_t of user
         */
        uid_t get_uid();
        /**
         * \brief Returns gid of User
         * \return Gid_t of user
         */
        gid_t get_gid();
        /**
         * \brief Returns user's name of User
         * \return String user name
         */
        std::string get_user_name();
        /**
         * \brief Returns info about the User
         * \return String user info
         */
        std::string get_user_info();
        /**
         * \brief Returns the home directory of user
         * \return String home directory
         */
        std::string get_home_dir();
        /**
         * \brief Returns the user login shell
         * \return String login shell
         */
        std::string get_shell_program();
        /**
         * \brief Returns if the class has been initialized
         * \return Bool Has User been initialized
         */
        bool is_initialized();
        /**
         * \brief Returns exit code of getpwnam_r 
         * \return Int exit code of getpwnam_r
         */
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
#endif