#ifndef TIME_VIEW_HPP
#define TIME_VIEW_HPP

#include <fleropp/IView.hpp>

class TimeView : public IView<TimeView> {
  public:
    TimeView() = default;
    ~TimeView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* TIME_VIEW_HPP */