#ifndef TABLE_EXAMPLE_VIEW_HPP
#define TABLE_EXAMPLE_VIEW_HPP

#include <fleropp/IView.hpp>

class TableExampleView : public IView<TableExampleView> {
  public:
    TableExampleView() = default;
    ~TableExampleView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* TABLE_EXAMPLE_VIEW_HPP */