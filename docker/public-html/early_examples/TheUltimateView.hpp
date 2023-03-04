#ifndef THE_ULTIMATE_VIEW_HPP
#define THE_ULTIMATE_VIEW_HPP

#include <fleropp/IView.hpp>

class TheUltimateView : public IView<TheUltimateView> {
  public:
    TheUltimateView() = default;
    ~TheUltimateView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* THE_ULTIMATE_VIEW_HPP */