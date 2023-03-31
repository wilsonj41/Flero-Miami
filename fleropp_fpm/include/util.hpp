#ifndef UTIL_HPP
#define UTIL_HPP

/**
 * \file util.hpp
 * \brief Various utility free functions and typedefs. 
 */

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <variant>
#include "Group.hpp"
#include <grp.h>
#include <pwd.h>

/**
 * \namespace fleropp::util 
 */
namespace fleropp::util {
    // The base class type
    template <class Base, typename... Args>
    using base_member_t = void (Base::*)(Args...);
    // The derived class type
    template <class Derived, typename... Args>
    using derived_member_t = void (Derived::*)(Args...);
    // A std::variant holding either the base class or the derived class
    template <class Base, class Derived, typename... Args>
    using member_variant_t = std::variant<base_member_t<Base, Args...>, derived_member_t<Derived, Args...>>;
    // A compile-time map holding a pointer-to-member-function and corresponding
    // textual representation
    template <class Base, class Derived, std::size_t N, typename... Args>
    using member_map_t = std::array<std::pair<member_variant_t<Base, Derived, Args...>, std::string_view>, N>;

    /**
     * \brief Count the number of overridden member functions at compile time.
     * 
     * Given a compile-time map of pointers-to-member functions to
     * their string representations for a CRTP class, this function
     * counts the number of them that have been overridden.
     * 
     * The parameters for each of the member functions must be
     * the same.
     * 
     * \tparam Base The type of the base class 
     * \tparam Derived The type of the derived class 
     * \tparam N The length of the map of member functions.
     * \tparam Args The arguments to each of the member functions.
     * 
     * \param[in] mmap The map of member function pointers to their
     *                 string representations.
     * 
     * \return The number of overridden member functions.
     */
    template <class Base, class Derived, std::size_t N, typename... Args>
    consteval std::size_t count_overridden(const member_map_t<Base, Derived, N, Args...>& mmap) {
        return std::count_if(std::cbegin(mmap), std::cend(mmap),
                            [] (const auto& pair) {
                                // If the type of pair.first (the member function) is of the
                                // derived class, then we know it has been overridden.
                                return std::holds_alternative<derived_member_t<Derived, Args...>>(pair.first); 
                            });
    }
    
    /**
     * \brief List the overridden member functions at compile time.
     * 
     * Given a compile-time map of pointers-to-member functions to
     * their string representations for a CRTP class, this function
     * provides a `std::array<std::string_view>` containing the string
     * representations of only those that have been overridden.
     * 
     * The parameters for each of the member functions must be
     * the same.
     * 
     * \tparam Base The type of the base class 
     * \tparam Derived The type of the derived class 
     * \tparam N The length of the map of member functions.
     * \tparam M The number of overridden member functions.
     * \tparam Args The arguments to each of the member functions.
     * 
     * \param[in] mmap The map of member function pointers to their
     *                 string representations.
     * 
     * \return An array containing the names of only the overridden
     *         member functions.
     */
    template <class Base, class Derived, std::size_t N, std::size_t M, typename... Args>
    consteval std::array<std::string_view, M> get_overridden(const member_map_t<Base, Derived, N, Args...>& mmap) {
        // Lambda that determines, given a pair containing a member function
        // pointer and its textual representation, whether it has been
        // overridden. 
        const auto is_overridden = [] (const auto& pair) {
            // If the type of pair.first (the member function) is of the
            // derived class, then we know it has been overridden.
            return std::holds_alternative<derived_member_t<Derived, Args...>>(pair.first); 
        };

        // Lambda that, given a pair, returns the second element
        const auto get_second = [] (const auto& pair) { return pair.second; };

        // Using a view is the easiest way to filter out the names
        // of member functions that have not been overridden.
        auto filtered = mmap | std::views::filter(is_overridden) 
                             | std::views::transform(get_second);

        // We then copy the resulting contents into a std::array,
        // as we want to return a container that can be returned 
        // and used outside of the `constexpr` context (e.g., via
        // assignment to a `static constexpr` variable that may
        // be printed at runtime). Once c++23 gets better compiler
        // support, `std::views::to` could do this instead.
        std::array<std::string_view, M> overridden;
        std::copy_n(filtered.begin(), M, std::begin(overridden));
        return overridden;

    }
    int daemonize();
    namespace permissions {
        /**
         * \brief Sets group of process to name of group
         * 
         * Given a string name of the group name the function
         * will add group to the process. If successful return true
         * otherwise returns false.
         * 
         * \param new_group The name of the group being added
         * 
         * \return Returns true if successfully added group, otherwise false
         */
        bool change_group_name(std::string new_group);
        /**
         * \brief Sets group of process to the gid of group
         * 
         * Given a gid of the group name the function
         * will add group to the process. If successful return true
         * otherwise returns false.
         * \param gid Gid of the group being added
         * 
         * \return Returns true if successfully added group, otherwise false
         */
        bool change_group_gid(gid_t gid);
        /**
         * \brief Sets user of process with the name of the user
         * 
         * Given a string of the user's name the function
         * will set the user to the process. If successful return true
         * otherwise returns false.
         * 
         * \param new_user The name of the user being set
         * 
         * \return Returns true if successfully set user, otherwise false
         */
        bool change_user_name(std::string new_user);
        /**
         * \brief Sets user of process with the name of the user
         * 
         * Given a uid of the user, the function
         * will set the user to the process. If successful return true
         * otherwise returns false.
         * 
         * \param new_user The name of the user being set
         * 
         * \return Returns true if successfully set user, otherwise false
         */
        bool change_user_uid(uid_t gid);

    }
}
#endif /* UTIL_HPP */