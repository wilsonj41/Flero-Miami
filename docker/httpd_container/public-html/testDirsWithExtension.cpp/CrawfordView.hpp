#ifndef CRAWFORD_VIEW_HPP
#define CRAWFORD_VIEW_HPP

#include <fleropp/IView.hpp>

class CrawfordView : public IView<CrawfordView> {
  public:
    CrawfordView() = default;
    ~CrawfordView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* CRAWFORD_VIEW_HPP */