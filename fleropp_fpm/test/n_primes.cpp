#include <string>
#include <vector>


using ullint_t = unsigned long long int;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    std::vector<ullint_t> primes{2ULL};
    const auto N = std::stoul(argv[1]);
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
}
