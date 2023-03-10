#ifndef TABLE_EXAMPLE_WITH_BORDER_VIEW_HPP
#define TABLE_EXAMPLE_WITH_BORDER_VIEW_HPP

#include <fleropp/IView.hpp>

class TableExampleWithBorderView : public IView<TableExampleWithBorderView> {
  public:
    TableExampleWithBorderView() = default;
    ~TableExampleWithBorderView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* TABLE_EXAMPLE_WITH_BORDER_VIEW_HPP */