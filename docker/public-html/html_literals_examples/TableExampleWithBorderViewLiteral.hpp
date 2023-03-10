#ifndef TABLE_EXAMPLE_WITH_BORDER_VIEW_LITERAL_HPP
#define TABLE_EXAMPLE_WITH_BORDER_VIEW_LITERAL_HPP

#include <fleropp/IView.hpp>

class TableExampleWithBorderViewLiteral : public IView<TableExampleWithBorderViewLiteral> {
  public:
    TableExampleWithBorderViewLiteral() = default;
    ~TableExampleWithBorderViewLiteral() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* TABLE_EXAMPLE_WITH_BORDER_VIEW_LITERAL_HPP */