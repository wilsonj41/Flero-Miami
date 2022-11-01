#ifndef I_VIEW_HPP
#define I_VIEW_HPP

//#include "HTMLLiterals.hpp"

#include <ostream>

class IView {
  public:
    virtual ~IView() = default;
    virtual void generate() = 0;
};

#endif /* I_VIEW_HPP */