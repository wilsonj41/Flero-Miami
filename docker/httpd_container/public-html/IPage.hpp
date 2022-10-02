#ifndef I_PAGE_HPP
#define I_PAGE_HPP

#include <ostream>

class IPage {
  public:
    virtual ~IPage() = default;
    virtual void generate(std::ostream &os) = 0;
};

#endif /* I_PAGE_HPP */