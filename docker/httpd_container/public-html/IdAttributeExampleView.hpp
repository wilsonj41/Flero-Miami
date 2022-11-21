#ifndef ID_ATTRIBUTE_EXAMPLE_VIEW_HPP
#define ID_ATTRIBUTE_EXAMPLE_VIEW_HPP

#include "IView.hpp"

class IdAttributeExampleView : public IView {
  public:
    IdAttributeExampleView() = default;
    ~IdAttributeExampleView() = default;
    void generate() override;
};

#endif /* ID_ATTRIBUTE_EXAMPLE_VIEW_HPP */