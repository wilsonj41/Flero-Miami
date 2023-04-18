#ifndef N_PRIMES_VIEW_HPP
#define N_PRIMES_VIEW_HPP

#include <fleropp/IView.hpp>

class NPrimesView : public IView<NPrimesView> {
  public:
    NPrimesView() = default;
    ~NPrimesView() = default;
    void get(const fleropp::io::RequestData& request);
};

#endif /* N_PRIMES_VIEW_HPP */