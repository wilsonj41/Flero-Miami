#ifndef TABLE_EXAMPLE_VIEW_LITERAL_HPP
#define TABLE_EXAMPLE_VIEW_LITERAL_HPP

#include <fleropp/IView.hpp>

class TableExampleViewLiteral : public IView<TableExampleViewLiteral> {
  public:
    TableExampleViewLiteral() = default;
    ~TableExampleViewLiteral() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* TABLE_EXAMPLE_VIEW_LITERAL_HPP */