#ifndef ID_ATTRIBUTE_EXAMPLE_VIEW_HPP
#define ID_ATTRIBUTE_EXAMPLE_VIEW_HPP

#include <fleropp/IView.hpp>

class IdAttributeExampleView : public IView<IdAttributeExampleView> {
  public:
    IdAttributeExampleView() = default;
    ~IdAttributeExampleView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* ID_ATTRIBUTE_EXAMPLE_VIEW_HPP */