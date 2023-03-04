#ifndef IFRAME_EXAMPLE_VIEW_LITERAL_HPP
#define IFRAME_EXAMPLE_VIEW_LITERAL_HPP

#include <fleropp/IView.hpp>

class IframeExampleViewLiteral : public IView {
  public:
    IframeExampleViewLiteral() = default;
    ~IframeExampleViewLiteral() = default;
    void generate() override;
};

#endif /* IFRAME_EXAMPLE_VIEW_LITERAL_HPP */