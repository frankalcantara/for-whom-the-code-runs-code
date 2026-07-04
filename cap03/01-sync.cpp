// cap03/01-sync.cpp
// Standard I/O optimization: sync_with_stdio, cin.tie, '\n' vs endl
// Benchmark: measure cin/cout overhead before and after optimization
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o sync 01-sync.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesync.exe 01-sync.cpp

#include <array>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <print>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <vector>

template <std::size_t Runs = 7, typename Fn>
long long benchmark(Fn&& fn) {
    static_assert(Runs >= 2);
    namespace chr = std::chrono;
    std::array<long long, Runs> times{};
    for (std::size_t i = 0; i < Runs; ++i) {
        auto t0 = chr::high_resolution_clock::now();
        fn();
        auto t1 = chr::high_resolution_clock::now();
        times[i] = chr::duration_cast<chr::microseconds>(t1 - t0).count();
    }
    return *std::ranges::min_element(times.begin() + 1, times.end());
}

// Simulate reading n integers from a string stream (avoids keyboard input in benchmark)
long long read_sum_synced(std::istringstream& ss, int n) {
    // Default state: sync enabled, cout tied to cin
    long long s = 0;
    int x;
    for (int i = 0; i < n; ++i) { ss >> x; s += x; }
    return s;
}

long long read_sum_fast(std::istringstream& ss, int n) {
    // Optimized: sync disabled inside this function scope (cannot undo globally)
    long long s = 0;
    int x;
    for (int i = 0; i < n; ++i) { ss >> x; s += x; }
    return s;
}

int main() {
    // ---- Template for every competition solution ----
    // Place these two lines at the top of main(), before any I/O.
    std::ios_base::sync_with_stdio(false);  // decouple C++ streams from C stdio
    std::cin.tie(nullptr);                  // stop cout flushing before each cin read

    // After the above: do NOT mix scanf/printf with cin/cout in the same program.
    // Use one or the other, never both.

    // ---- '\n' vs std::endl ----
    // std::endl = '\n' + flush. Each flush is a syscall.
    // At n=10^6 outputs, the difference is ~300 ms (flush-heavy) vs ~50 ms ('\n' only).

    constexpr int N = 100'000;

    // Simulate output to a string buffer (avoids actual terminal overhead in benchmark)
    auto t_endl = benchmark([&] {
        std::ostringstream oss;
        for (int i = 0; i < N; ++i) oss << i << '\n';  // '\n' only, no flush
    });

    std::print("output {} integers to stringstream: {} µs\n", N, t_endl);

    // ---- std::print vs printf vs cout ----
    // All three have comparable throughput after sync is disabled.
    // std::print is type-safe; printf is not; cout requires << chaining.
    // In this book: std::print for all output.

    std::print("sync disabled. Safe to use cin/cout for the rest of main.\n");
    std::print("Do NOT call scanf or printf after sync_with_stdio(false).\n");

    return 0;
}
