#ifndef STRING_LITERAL_HPP
#define STRING_LITERAL_HPP

#include <algorithm>
#include <array>

namespace fleropp::util {
    /**
    * \brief A string literal that is usable as a non-type template parameter. 
    * 
    * Per the C++ Standard, a non-type template parameter
    * must have "structural type", hence we create this class representing a
    * string that fits those requirements:
    *    - all base classes and non-static data members are public and 
    *      non-mutable and  
    *    - the types of all base classes and non-static data members
    *      are structural types or (possibly multi-dimensional) array thereof. 
    * \tparam N Length of the string literal.
    */  
    template<std::size_t N>
    struct StringLiteral {
        /**
         * Constant expression c-tor.
         * Accepts a reference to avoid decay of array to pointer.
         * 
         * \param[in] str Reference to a const char[N] containing the string data.
         */
        constexpr StringLiteral(const char (&str)[N]) {
            // Copy to N - 1 to strip null terminator
            // This overload of std::copy_n is constexpr in >= c++20
            std::copy_n(std::cbegin(str), N - 1, std::begin(value));
        }
        std::array<char, N - 1> value;
    };
}

#endif /* STRING_LITERAL_HPP */