#ifndef STATUS_CODES_HPP
#define STATUS_CODES_HPP

#include "FleroppIO.hpp"
#include "StringLiteral.hpp"

#include <string>
#include <string_view>

#include <fmt/compile.h>
#include <fmt/format.h>

namespace fleropp::util {
    /**
     * \brief Prints a status code and description as an HTTP response.
     * 
     * \tparam code A string literal representing the error code.
     * \tparam desc A string literal representing the error descriptio. 
     */
    template <StringLiteral code, StringLiteral desc>
    void status_response() {
        using namespace std::literals::string_view_literals;
        constexpr std::string_view response_str = "Status: {} {}\r\n"
                                                  "Content-Type: text/html\r\n"
                                                  "Content-Length: {}\r\n\r\n"
                                                  "<h1>{} {}</h1>\n";
        static constexpr std::string_view code_sv{std::cbegin(code.value), 
                                                  std::cend(code.value)};
        static constexpr std::string_view desc_sv{std::cbegin(desc.value), 
                                                  std::cend(desc.value)};
        static constexpr auto content_length = std::size(code_sv) + 
                                               std::size(desc_sv) + 
                                               std::size("<h1> </h1>\n"sv);
        fleropp::io::fppout << fmt::format(FMT_COMPILE(response_str), code_sv, desc_sv,
                                           content_length, code_sv, desc_sv);
    }
}

#endif /* STATUS_CODES_HPP */
