#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include <ostream>
#include <string_view>

#ifdef FPM_BUILD
#include "ConstexprMap.hpp"
#include "FleroppIO.hpp"
#include "RequestData.hpp"
#include "util.hpp"
#else
#include <fleropp/ConstexprMap.hpp>
#include <fleropp/FleroppIO.hpp>
#include <fleropp/RequestData.hpp>
#include <fleropp/util.hpp>
#endif

#include <fmt/format.h>

#define INIT_VIEW(NAME)                       \
extern "C" {                                  \
    NAME* allocator() {                       \
        return new NAME();                    \
    }                                         \
    void deleter(NAME* ptr) {                 \
        delete ptr;                           \
    }                                         \
}                                             \

struct IViewWrapper {
    virtual ~IViewWrapper() = default;
    virtual void get_dispatch(const fleropp_io::RequestData& request) = 0;
    virtual void post_dispatch(const fleropp_io::RequestData& request) = 0;
    virtual void put_dispatch(const fleropp_io::RequestData& request) = 0;
    virtual void del_dispatch(const fleropp_io::RequestData& request) = 0;
    virtual void get(const fleropp_io::RequestData& request) = 0;
    virtual void post(const fleropp_io::RequestData& request) = 0;
    virtual void put(const fleropp_io::RequestData& request) = 0;
    virtual void del(const fleropp_io::RequestData& request) = 0;
};

template <class DerivedView>
class IView : public IViewWrapper {
    using member_arg_t = const fleropp_io::RequestData&;
    using view_base_member_t = base_member_t<IView, member_arg_t>;
    using view_derived_member_t = derived_member_t<DerivedView, member_arg_t>;
    using view_member_variant_t = member_variant_t<IView, DerivedView, member_arg_t>;
    using view_member_map_t = member_map_t<IView, DerivedView, 4, member_arg_t>;
    
  public:
    virtual ~IView() = default;
    
    void get_dispatch(const fleropp_io::RequestData& request) final {
        static_cast<DerivedView*>(this)->get(request);
    }

    void post_dispatch(const fleropp_io::RequestData& request) final {
        static_cast<DerivedView*>(this)->post(request);
    }

    void put_dispatch(const fleropp_io::RequestData& request) final {
        static_cast<DerivedView*>(this)->put(request);
    }

    void del_dispatch(const fleropp_io::RequestData& request) final {
        static_cast<DerivedView*>(this)->del(request);
    }

    void get(const fleropp_io::RequestData& request) { print_supported(); }

    void post(const fleropp_io::RequestData& request) { print_supported(); }

    void put(const fleropp_io::RequestData& request) { print_supported(); }

    void del(const fleropp_io::RequestData& request) { print_supported(); }
  private:
    struct constants {
        static constexpr view_member_map_t members{{{&DerivedView::get, "GET"}, 
                                                    {&DerivedView::post, "POST"},
                                                    {&DerivedView::put, "PUT"},
                                                    {&DerivedView::del, "DELETE"}}};
        static constexpr std::string_view default_response = 
            "Status: 405 Method Not Allowed\r\n"
            "Content-type: text/html\r\n"
            "Allow: {}\r\n"
            "Content-length: 27\r\n\r\n"
            "<h1>Method Not Allowed</h1>";
        static constexpr auto n_supported = 
            count_overridden<IView, DerivedView, 4, member_arg_t>(members);
        static constexpr auto supported = 
            supported_methods<IView, DerivedView, 4, n_supported, member_arg_t>(members); 
    };

    static void print_supported() {
        fleropp_io::fppout << fmt::format(constants::default_response, 
                                          fmt::join(constants::supported, ", "));
    }
};

#endif /* I_VIEW_HPP */