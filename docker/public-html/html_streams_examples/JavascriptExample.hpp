#ifndef JAVASCRIPT_EXAMPLE_HPP
#define JAVASCRIPT_EXAMPLE_HPP

#include <fleropp/IView.hpp>

class JavascriptExample : public IView<JavascriptExample> {
  public:
    JavascriptExample() = default;
    ~JavascriptExample() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* JAVASCRIPT_EXAMPLE_HPP */