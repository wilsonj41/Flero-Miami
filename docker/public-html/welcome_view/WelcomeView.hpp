#ifndef WELCOME_VIEW_HPP
#define WELCOME_VIEW_HPP

#include <fleropp/IView.hpp>

class WelcomeView : public IView<WelcomeView> {
  public:
    WelcomeView() = default;
    ~WelcomeView() = default;

    /* We need to specify the HTTP methods that we want to use in our web page.*/
    void get(const fleropp::io::RequestData& request); 
    void post(const fleropp::io::RequestData& request);
};

#endif /* WELCOME_VIEW_HPP */