#ifndef JAVASCRIPT_EXAMPLE_HPP
#define JAVASCRIPT_EXAMPLE_HPP

#include <fleropp/IView.hpp>

class JavascriptExample : public IView {
  public:
    JavascriptExample() = default;
    ~JavascriptExample() = default;
    void generate() override;
};

#endif /* JAVASCRIPT_EXAMPLE_HPP */