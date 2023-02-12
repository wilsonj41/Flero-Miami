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

struct IViewInterface {
    virtual ~IViewInterface() = default;
    virtual void get() = 0;
    virtual void post() = 0;
    virtual void post_impl() = 0;
    virtual void get_impl() = 0;
};

template<class DerivedView>
class IView : IViewInterface {
  public:
    virtual ~IView() = default;
    void get() {
        static_cast<DerivedView*>(this)->get_impl();
    }
    void post() {
        static_cost<DerivedView*>(this)->post_impl();
    }
    virtual void generate() = 0;
    void get_impl() {}
    void post_imp() {}
};

#endif /* I_VIEW_HPP */