#ifndef TABLE_EXAMPLE_WITH_BORDER_VIEW_HPP
#define TABLE_EXAMPLE_WITH_BORDER_VIEW_HPP

#include "IView.hpp"

class TableExampleWithBorderView : public IView {
  public:
    TableExampleWithBorderView() = default;
    ~TableExampleWithBorderView() = default;
    void generate() override;
};

#endif /* TABLE_EXAMPLE_WITH_BORDER_VIEW_HPP */