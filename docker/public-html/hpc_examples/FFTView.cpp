#include <algorithm>
#include <cgicc/HTMLClasses.h>
#include <cmath>
#include <complex>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>
#include <numeric>
#include <vector>

#include "FFTView.hpp"

INIT_VIEW(FFTView)

namespace fft_util {
    constexpr double PI = 3.1415926536;

    // Bit-reversal permutation, as a given output index of the FFT alglorithm is a
    // bit-reversal of the input index.
    static unsigned int bit_reverse(unsigned int x, int log2n) {
        unsigned int n = 0;
        for (int i = 0; i < log2n; ++i, x >>= 1) {
            n = (n <<= 1) | (x & 1U);
        }
        return n;
    }

    // Computes the fourier transform of a vector using
    // the decimation-in-time radix-2 FFT algorithm
    static auto fft(const std::vector<std::complex<double>>& a) {
        // J = i
        const std::complex<double> J(0, 1);

        const int n = a.size();
        const int log2n = std::log2(n);
        std::vector<std::complex<double>> b(n);

        // Build roots of unity LUT
        std::vector<std::complex<double>> wn(n / 2);
#pragma omp parallel for shared(wn, n)
        for (std::size_t i = 0; i < n / 2; ++i) {
            wn[i] = std::exp(J * (-2 * PI * i / n));
        }

#pragma omp parallel for shared(a, b)
        for (std::size_t i = 0; i < n; ++i) {
            b[bit_reverse(i, log2n)] = a[i];
        }

#pragma omp parallel shared(b)
    {
        for (std::size_t s = 2; s <= n; s *= 2) {
            int s2 = s / 2;
            // Periodicity for accessing roots of unity
            int r = n / s;
#pragma omp for
            for (std::size_t i = 0; i < n; i += s) {
                for (std::size_t j = i, k = 0; j < i + s2; ++j, k += r) {
                    // Twiddle factor
                    // The value of t is periodic (with period n/2),
                    // hence it must only be calculated up to n/2
                    auto t = wn[k] * b[j + s2];

                    // Rotating by n/2 is just negation
                    b[j + s2] = b[j] - t;
                    // Evaluate at the current root of unity
                    b[j] += t;
                }
            }

            if (s == n) {
                break;
            }
        }
    }
        return b;
    }
}

void FFTView::get(const fleropp::io::RequestData& request) {
    using namespace fft_util;
    using namespace fleropp::literals;
    namespace htmls = fleropp::html_stream;

    std::vector<std::complex<double>> time(1UL << 14);
    // Generate symmetric input data
    std::generate_n(std::begin(time), time.size() / 2,
                    [z = std::complex<double>{0, 0}]() mutable {
                        return z += std::complex<double>{1, 1};
                    });

    std::generate_n(std::rbegin(time), time.size() / 2, 
                    [z = std::complex<double>{0, 0}]() mutable {
                        return z += std::complex<double>{1, 1};
                    });
    
    const auto freq = fft(time);

    auto print_complex = []<typename T>(const std::complex<T>& z) {
        "<td>{} + {}<i>i</i></td>"_f(z.real(), z.imag());
    };

    htmls::gen_html_boilerplate("text/html");
    "<html>"_h;
        "<head>"_h;
            R"(<style>
                table {
	                display: table;
                }
                table tr {
	                display: table-cell;
                }
                table tr td {
	                display: block;
                }
                table, th, td {
                    border: 1px solid black;
                    border-collapse: collapse;
                }
                th, td {
                    padding: 15px;
                }
            </style>)"_h;
        "</head>"_h;
        "<body>"_h;
            "<h1>Decimation-in-time Radix-2 Fast Fourier Transform</h2>"_h;
            "<h2>Parallelized with OpenMP</h2>"_h;
            "<table>"_h;
                "<tr>"_h;
                    "<td><b>Input</b></td>"_h;
                    for (const auto& z : time) {
                        print_complex(z);
                    }
                "</tr>"_h;
                "<tr>"_h;
                    "<td><b>Output</b></td>"_h;
                    for (const auto& z : freq) {
                        print_complex(z);
                    }
                "</tr>"_h;
            "</table>"_h;
        "</body>"_h;
    "</html>"_h;
}
