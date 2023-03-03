#include <array>
#include <cgicc/HTMLClasses.h>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <ranges>
#include <string_view>
#include <tuple>

#include "TableExampleWithBorderViewLiteral.hpp"

// link to reference: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_basic

INIT_VIEW(TableExampleWithBorderViewLiteral)

void TableExampleWithBorderViewLiteral::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    namespace htmls = fleropp::html_stream;
    using entry = std::tuple<std::string_view, std::string_view, unsigned int>;

    htmls::gen_html_boiler_plate("text/html");
    "<html>"_h;
        "<head>"_h;
            // Raw string literals are fully supported
            R"(<style>
                table, th, td {
                    border: 1px solid black;
                    border-collapse: collapse;
                }
            </style>)"_h;
        "</head>"_h;
        "<body>"_h;

            "<h2>Collapsed Borders</h2>"_h;
            "<p>If you want the borders to collapse into one border, "
                "add the CSS border-collapse property.</p>"_h;

            static constexpr std::array<entry, 3> people = {entry{"Jill", "Smith", 50},
                                                            entry{"Eve", "Jackson", 94},
                                                            entry{"John", "Doe", 80}};

            // Apply HTML formatting to a 3-tuple of fields
            auto print_row = []<typename... Args>(Args&& ...args) {
                R"(<tr>
                    <td>{}</td>
                    <td>{}</td>
                    <td>{}</td>
                </tr>)"_f(std::forward<Args>(args)...);
            };

            "<table style='width:100%'>"_h;
                R"(<tr>
                    <th>Firstname</th>
                    <th>Lastname</th>
                    <th>Age</th>
                </tr>)"_h;
                for (auto& row : people) {
                    std::apply(print_row, row);
                }
            "</table>"_h;

        "</body>"_h;
    "</html>"_h;
}
