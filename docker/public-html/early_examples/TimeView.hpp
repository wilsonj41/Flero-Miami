#ifndef TIME_VIEW_HPP
#define TIME_VIEW_HPP

#include <fleropp/IView.hpp>

class TimeView : public IView {
  public:
    TimeView() = default;
    ~TimeView() = default;
    void generate() override;
};

#endif /* TIME_VIEW_HPP */