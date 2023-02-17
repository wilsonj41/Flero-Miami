#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <variant>

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

template <class Base, class Derived, std::size_t N, typename... Args>
static constexpr std::size_t count_overridden(const member_map_t<Base, Derived, N, Args...>& mmap) {
    return std::count_if(std::cbegin(mmap), std::cend(mmap),
                        [] (const auto& pair) { 
                            return std::holds_alternative<derived_member_t<Derived, Args...>>(pair.first); 
                        });
}

template <class Base, class Derived, std::size_t N, std::size_t M, typename... Args>
static constexpr std::array<std::string_view, M> supported_methods(const member_map_t<Base, Derived, N, Args...>& mmap) {
    const auto is_overridden = [] (const auto& pair) {
        return std::holds_alternative<derived_member_t<Derived, Args...>>(pair.first); 
    };

    const auto get_second = [] (const auto& pair) { return pair.second; };
    auto filtered = mmap | std::views::filter(is_overridden) 
                         | std::views::transform(get_second);
    std::array<std::string_view, M> supported;
    std::copy_n(filtered.begin(), M, std::begin(supported));
    return supported;
}

#endif /* UTIL_HPP */