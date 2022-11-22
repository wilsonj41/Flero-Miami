#ifndef JOEY_VIEW_HPP
#define JOEY_VIEW_HPP

#include "IView.hpp"

class JoeyView : public IView {
  public:
    JoeyView() = default;
    ~JoeyView() = default;
    void generate() override;
};

#endif /* JOEY_VIEW_HPP */