#ifndef CLASS_ATTRIBUTE_EXAMPLE_VIEW_HPP
#define CLASS_ATTRIBUTE_EXAMPLE_VIEW_HPP

#include "IView.hpp"

class ClassAttributeExampleView : public IView {
  public:
    ClassAttributeExampleView() = default;
    ~ClassAttributeExampleView() = default;
    void generate() override;
};

#endif /* CLASS_ATTRIBUTE_EXAMPLE_VIEW_HPP */