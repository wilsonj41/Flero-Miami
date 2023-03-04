#ifndef MULTIPART_POST_VIEW_HPP
#define MULTIPART_POST_VIEW_HPP

#include <fleropp/IView.hpp>

class MultiPartPostView : public IView<MultiPartPostView> {
  public:
    MultiPartPostView() = default;
    ~MultiPartPostView() = default;
    void get(const fleropp::io::RequestData& request);
    void post(const fleropp::io::RequestData& request);
};

#endif /* MULTIPART_POST_VIEW_HPP */