#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <variant>

// fleropp utility namespace
namespace fleropp_util {
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
     * \brief Count the number of overridden member functions.
     * 
     * Given a compile-time map of pointers-to-member functions to
     * their string representations for a CRTP class, this function
     * counts the number of them that have been overridden.
     * 
     * The parameters for each of the member functions must be
     * the same.
     * 
     * \tparam Base The type of the base class 
     * \tparam Derived The type of the base class 
     * \tparam N The length of the map of member functions.
     * \tparam Args The arguments to each of the member functions.
     * 
     * \param[in] mmap The map of member function pointers to their
     *                 string representations.
     * 
     * \return The number of overridden member functions.
    */
    template <class Base, class Derived, std::size_t N, typename... Args>
    static constexpr std::size_t count_overridden(const member_map_t<Base, Derived, N, Args...>& mmap) {
        return std::count_if(std::cbegin(mmap), std::cend(mmap),
                            [] (const auto& pair) {
                                // If the type of pair.first (the member function) is of the
                                // derived class, then we know it has been overridden.
                                return std::holds_alternative<derived_member_t<Derived, Args...>>(pair.first); 
                            });
    }
    
    /**
     * \brief List the overridden member functions.
     * 
     * Given a compile-time map of pointers-to-member functions to
     * their string representations for a CRTP class, this function
     * provides a std::array of std::string_view containing the string
     * representations of only those that have been overridden.
     * 
     * The parameters for each of the member functions must be
     * the same.
     * 
     * \tparam Base The type of the base class 
     * \tparam Derived The type of the base class 
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
    static constexpr std::array<std::string_view, M> supported_methods(const member_map_t<Base, Derived, N, Args...>& mmap) {
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
        // be printed at runtime).
        std::array<std::string_view, M> supported;
        std::copy_n(filtered.begin(), M, std::begin(supported));
        return supported;
    }
}
#endif /* UTIL_HPP */