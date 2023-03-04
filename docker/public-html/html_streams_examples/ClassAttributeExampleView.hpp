#ifndef CLASS_ATTRIBUTE_EXAMPLE_VIEW_HPP
#define CLASS_ATTRIBUTE_EXAMPLE_VIEW_HPP

#include <fleropp/IView.hpp>

class ClassAttributeExampleView : public IView<ClassAttributeExampleView> {
  public:
    ClassAttributeExampleView() = default;
    ~ClassAttributeExampleView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* CLASS_ATTRIBUTE_EXAMPLE_VIEW_HPP */