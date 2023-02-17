#ifndef DISPATCH_HPP
#define DISPATCH_HPP

#include <array>

#ifdef FPM_BUILD
#include "ConstexprMap.hpp"
#include "IView.hpp"
#include "RequestData.hpp"
#else
#include <fleropp/ConstexprMap.hpp>
#include <fleropp/IView.hpp>
#include <fleropp/RequestData.hpp>
#endif

namespace fleropp_fpm::dispatch {
    // Represents a pointer to a member funtion of the view wrapper class 
    // this wrapper is used for dispatch only.
    using wrapper_member_t = void (IViewWrapper::*)(const fleropp_io::RequestData&);

    // Values to be inserted into the map: HTTP method name to wrapper dispatch
    // function. 
    static constexpr std::array<std::pair<std::string_view, wrapper_member_t>, 4> request_vals{{{"GET", &IViewWrapper::get_dispatch}, 
                                                                                                {"POST", &IViewWrapper::post_dispatch},
                                                                                                {"PUT", &IViewWrapper::put_dispatch},
                                                                                                {"DELETE", &IViewWrapper::del_dispatch}}};

    // A compile-time mapping of HTTP functions to their corresponding
    // handlers.                                                                                            
    static constexpr auto request_dispatch_funs = 
            ConstexprMap<std::string_view, wrapper_member_t, request_vals.size()>{{request_vals}};
}

#endif /* DISPATCH_HPP */