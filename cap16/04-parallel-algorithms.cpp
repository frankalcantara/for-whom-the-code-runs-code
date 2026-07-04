// cap16/04-parallel-algorithms.cpp
// STL execution policies: seq, par, and par_unseq.
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feparallel_algorithms.exe 04-parallel-algorithms.cpp

#include <cassert>
#include <execution>
#include <functional>
#include <numeric>
#include <print>
#include <vector>

long long dot_product_seq(const std::vector<int>& a, const std::vector<int>& b) {
    return std::transform_reduce(
        std::execution::seq,
        a.begin(), a.end(), b.begin(),
        0LL,
        std::plus<long long>{},
        [](int x, int y) { return 1LL * x * y; });
}

long long dot_product_par_unseq(const std::vector<int>& a, const std::vector<int>& b) {
    return std::transform_reduce(
        std::execution::par_unseq,
        a.begin(), a.end(), b.begin(),
        0LL,
        std::plus<long long>{},
        [](int x, int y) { return 1LL * x * y; });
}

int main() {
    std::vector<int> a(100'000, 2), b(100'000, 3);
    long long seq = dot_product_seq(a, b);
    long long par = dot_product_par_unseq(a, b);
    assert(seq == 600'000);
    assert(par == seq);

    std::print("04-parallel-algorithms: all assertions passed\n");
    return 0;
}
