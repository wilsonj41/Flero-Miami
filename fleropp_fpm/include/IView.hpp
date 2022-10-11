#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include <ostream>

class IView {
  public:
    virtual ~IView() = default;
    /**
     * Renders the content of the view that 
     * will be sent
     * 
     * \param os Output stream that will render content
     */
    virtual void generate(std::ostream &os) = 0;
};

#endif /* I_VIEW_HPP */