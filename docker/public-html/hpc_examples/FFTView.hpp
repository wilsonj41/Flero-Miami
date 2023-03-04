#ifndef FFT_VIEW_HPP
#define FFT_VIEW_HPP

#include <fleropp/IView.hpp>
#include <omp.h>

class FFTView : public IView {
  public:
    FFTView() { omp_set_num_threads(12); };
    ~FFTView() = default;
    void generate() override;
};

#endif /* FFT_VIEW_HPP */