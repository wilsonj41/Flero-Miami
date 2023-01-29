#ifndef TABLE_EXAMPLE_VIEW_LITERAL_HPP
#define TABLE_EXAMPLE_VIEW_LITERAL_HPP

#include <fleropp/IView.hpp>

class TableExampleViewLiteral : public IView {
  public:
    TableExampleViewLiteral() = default;
    ~TableExampleViewLiteral() = default;
    void generate() override;
};

#endif /* TABLE_EXAMPLE_VIEW_LITERAL_HPP */