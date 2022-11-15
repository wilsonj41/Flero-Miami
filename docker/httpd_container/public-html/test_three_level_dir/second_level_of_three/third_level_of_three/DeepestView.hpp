#ifndef DEEPEST_VIEW_HPP
#define DEEPEST_VIEW_HPP

#include "../../../IView.hpp"

class DeepestView : public IView {
  public:
    DeepestView() = default;
    ~DeepestView() = default;
    void generate() override;
};

#endif /* DEEPEST_VIEW_HPP */