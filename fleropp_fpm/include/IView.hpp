#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include <ostream>

#define INIT_VIEW(NAME)                       \
extern "C" {                                  \
    NAME* allocator() {                       \
        return new NAME();                    \
    }                                         \
    void deleter(NAME* ptr) {                 \
        delete ptr;                           \
    }                                         \
}                                             \

class IView {
  public:
    virtual ~IView() = default;
    virtual void generate() = 0;
};

#endif /* I_VIEW_HPP */