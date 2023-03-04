#ifndef JOEY_VIEW_HPP
#define JOEY_VIEW_HPP

#include <fleropp/IView.hpp>

class JoeyView : public IView<JoeyView> {
  public:
    JoeyView() = default;
    ~JoeyView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* JOEY_VIEW_HPP */