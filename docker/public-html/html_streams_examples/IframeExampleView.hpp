#ifndef IFRAME_EXAMPLE_VIEW_HPP
#define IFRAME_EXAMPLE_VIEW_HPP

#include <fleropp/IView.hpp>

class IframeExampleView : public IView<IframeExampleView> {
  public:
    IframeExampleView() = default;
    ~IframeExampleView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* IFRAME_EXAMPLE_VIEW_HPP */