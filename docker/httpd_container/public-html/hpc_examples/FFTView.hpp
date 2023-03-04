#ifndef FFT_VIEW_HPP
#define FFT_VIEW_HPP

#include <fleropp/IView.hpp>
#include <omp.h>

class FFTView : public IView<FFTView> {
  public:
    FFTView() { omp_set_num_threads(12); };
    ~FFTView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* FFT_VIEW_HPP */