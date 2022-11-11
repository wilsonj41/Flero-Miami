#ifndef THE_ULTIMATE_VIEW_HPP
#define THE_ULTIMATE_VIEW_HPP

#include "IView.hpp"

class TheUltimateView : public IView {
  public:
    TheUltimateView() = default;
    ~TheUltimateView() = default;
    void generate() override;
};

#endif /* THE_ULTIMATE_VIEW_HPP */