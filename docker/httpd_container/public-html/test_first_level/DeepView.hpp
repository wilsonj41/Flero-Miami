#ifndef DEEP_VIEW_HPP
#define DEEP_VIEW_HPP

#include "../IView.hpp"

class DeepView : public IView {
  public:
    DeepView() = default;
    ~DeepView() = default;
    void generate() override;
};

#endif /* DEEP_VIEW_HPP */