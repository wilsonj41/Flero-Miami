#ifndef CRAWFORD_VIEW_HPP
#define CRAWFORD_VIEW_HPP

#include <fleropp/IView.hpp>

class CrawfordView : public IView {
  public:
    CrawfordView() = default;
    ~CrawfordView() = default;
    void generate() override;
};

#endif /* CRAWFORD_VIEW_HPP */