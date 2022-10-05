#ifndef TIME_VIEW_HPP
#define TIME_VIEW_HPP

#include "IView.hpp"

class TimeView : public IView {
  public:
    TimeView() = default;
    ~TimeView() = default;
    void generate(std::ostream &os) override;
};

#endif /* HELLO_WORLD_View_HPP */