#ifndef HELLO_WORLD_VIEW_HPP
#define HELLO_WORLD_VIEW_HPP

#include <fleropp/IView.hpp>

class HelloWorldView : public IView<HelloWorldView> {
  public:
    HelloWorldView() = default;
    ~HelloWorldView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* HELLO_WORLD_VIEW_HPP */
