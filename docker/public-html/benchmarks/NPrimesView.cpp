#include "NPrimesView.hpp"
#include <fleropp/HTMLLiterals.hpp>

#include <vector>

using ullint_t = unsigned long long int;

INIT_VIEW(NPrimesView)

void NPrimesView::get(const fleropp::io::RequestData& request) {
    using namespace fleropp::literals;
    std::vector<ullint_t> primes{2ULL};
    const auto N = std::stoul(request.get_query_string().get("n"));
    primes.resize(N);
    std::size_t primes_count = 1;
    const auto is_prime = [&primes, &primes_count] (ullint_t n) {
        for (std::size_t i = 0; primes[i] * primes[i] <= n; ++i) {
            if (!(n % primes[i])) {
                return true;
            }
        }
        return false;
    };

    for (ullint_t n = 3; primes_count < N; ++n) {
        if (is_prime(n)) {
            primes[primes_count++] = n;
        }
    }

    "Done."_h;
}