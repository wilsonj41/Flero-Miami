#ifndef TABLE_EXAMPLE_WITH_BORDER_VIEW_LITERAL_HPP
#define TABLE_EXAMPLE_WITH_BORDER_VIEW_LITERAL_HPP

#include <fleropp/IView.hpp>

class TableExampleWithBorderViewLiteral : public IView {
  public:
    TableExampleWithBorderViewLiteral() = default;
    ~TableExampleWithBorderViewLiteral() = default;
    void generate() override;
};

#endif /* TABLE_EXAMPLE_WITH_BORDER_VIEW_LITERAL_HPP */