#ifndef HELLO_WORLD_PAGE_HPP
#define HELLO_WORLD_PAGE_HPP

#include "IPage.hpp"

class HelloWorldPage : public IPage {
  public:
    HelloWorldPage() = default;
    ~HelloWorldPage() = default;
    void generate(std::ostream &os) override;
};

#endif /* HELLO_WORLD_PAGE_HPP */