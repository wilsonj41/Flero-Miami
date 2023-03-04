#ifndef IFRAME_EXAMPLE_VIEW_LITERAL_HPP
#define IFRAME_EXAMPLE_VIEW_LITERAL_HPP

#include <fleropp/IView.hpp>

class IframeExampleViewLiteral : public IView<IframeExampleViewLiteral> {
  public:
    IframeExampleViewLiteral() = default;
    ~IframeExampleViewLiteral() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* IFRAME_EXAMPLE_VIEW_LITERAL_HPP */