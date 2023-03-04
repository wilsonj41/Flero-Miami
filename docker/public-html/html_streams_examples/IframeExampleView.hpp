#ifndef IFRAME_EXAMPLE_VIEW_HPP
#define IFRAME_EXAMPLE_VIEW_HPP

#include <fleropp/IView.hpp>

class IframeExampleView : public IView {
  public:
    IframeExampleView() = default;
    ~IframeExampleView() = default;
    void generate() override;
};

#endif /* IFRAME_EXAMPLE_VIEW_HPP */