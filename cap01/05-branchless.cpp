// cap01/05-branchless.cpp
// Branch prediction and branchless code. The same "sum the elements above a
// threshold" loop is timed on unsorted data (an unpredictable branch), on
// sorted data (a predictable branch), and as an explicit branchless variant.
// Note: at -O2 the compiler may already turn the branch into a conditional
// move; when it cannot, the sorted/unsorted gap is the cost of misprediction.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o branchless 05-branchless.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febranchless.exe 05-branchless.cpp

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

// A branch that resists conversion to a conditional move: the taken path does
// real, opaque work, so the compiler keeps the control-flow branch.
static long long sum_branchy(const std::vector<int>& a, int t) {
    long long s = 0;
    for (int x : a)
        if (x > t) s += static_cast<long long>(x) * x + (x ^ 0x5bd1e995);
    return s;
}

static long long sum_branchless(const std::vector<int>& a, int t) {
    long long s = 0;
    for (int x : a) {
        long long contrib = static_cast<long long>(x) * x + (x ^ 0x5bd1e995);
        s += contrib * (x > t);   // decision folded into arithmetic, no branch
    }
    return s;
}

template <class F>
static double time_ms(F&& f) {
    auto t0 = Clock::now();
    volatile long long sink = f();   // volatile keeps the work from being elided
    (void)sink;
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    const int n = 20'000'000;
    const int threshold = 500'000'000;

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0, 1'000'000'000);
    std::vector<int> data(n);
    for (int& x : data) x = dist(rng);

    double unsorted = time_ms([&] { return sum_branchy(data, threshold); });
    double bless1   = time_ms([&] { return sum_branchless(data, threshold); });

    std::ranges::sort(data);
    double sorted = time_ms([&] { return sum_branchy(data, threshold); });
    double bless2 = time_ms([&] { return sum_branchless(data, threshold); });

    std::println("branchy,    unsorted : {:.1f} ms", unsorted);
    std::println("branchy,    sorted   : {:.1f} ms", sorted);
    std::println("branchless, unsorted : {:.1f} ms", bless1);
    std::println("branchless, sorted   : {:.1f} ms", bless2);
    return 0;
}
