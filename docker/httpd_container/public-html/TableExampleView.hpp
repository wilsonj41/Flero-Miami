#ifndef TABLE_EXAMPLE_VIEW_HPP
#define TABLE_EXAMPLE_VIEW_HPP

#include "IView.hpp"

class TableExampleView : public IView {
  public:
    TableExampleView() = default;
    ~TableExampleView() = default;
    void generate() override;
};

#endif /* TABLE_EXAMPLE_VIEW_HPP */