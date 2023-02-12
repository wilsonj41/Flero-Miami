#ifndef HELLO_WORLD_VIEW_HPP
#define HELLO_WORLD_VIEW_HPP

#include <fleropp/IView.hpp>

class HelloWorldView : public IView {
  public:
    HelloWorldView() = default;
    ~HelloWorldView() = default;
    void generate(const fleropp_io::RequestData& request) override;
};

#endif /* HELLO_WORLD_VIEW_HPP */