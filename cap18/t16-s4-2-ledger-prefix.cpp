// Exercise T16.6 - The Ledger Prefix
// Session: T16.S4, exercise 2 of 3 (2 hours for all three)
// Theme: chapter 16 (parallel prefix by block decomposition)
// Tags: chapter-16, prefix-sum, parallelism
//
// MODEL: compute all prefix sums of an array by splitting it into
//   blocks. Each block first computes local prefixes; then block
//   offsets are added.
// MATH: local_prefix[i] inside block b is the sum from the block
//   start through i. The required global prefix is local_prefix[i]
//   plus the sum of all earlier blocks. Those block sums are
//   scanned sequentially because there are only p of them.
// PROOF: decomposition. Every prefix ending in block b consists of
//   the complete blocks before b plus the partial prefix inside b.
//   The first phase computes the partial prefix exactly. The offset
//   phase adds exactly the complete earlier block sums, so every
//   output entry equals the definition of prefix_i.
// COMPLEXITY: O(n + p) total work and O(n + p) memory. The two
//   parallel phases touch each element once.
// TYPES: prefix sums can reach 2e14 in magnitude, so int64_t is
//   used.
// ALTERNATIVES: a sequential scan is simpler and often faster for
//   small n; full tree-based scan is more parallel but longer.
// EDGE CASES: p > n; n = 1; negative values; zero block offsets.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -pthread -o prefix t16-s4-2-ledger-prefix.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feprefix.exe t16-s4-2-ledger-prefix.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int workers = 0;
    std::cin >> n >> workers;
    std::vector<std::int64_t> values(n);
    for (std::int64_t& value : values) std::cin >> value;

    workers = std::min(workers, n);
    std::vector<std::int64_t> prefix(n, 0);
    std::vector<std::int64_t> block_sum(workers, 0);
    std::vector<std::jthread> pool;

    for (int id = 0; id < workers; ++id) {
        const int lo = id * n / workers;
        const int hi = (id + 1) * n / workers;
        pool.emplace_back([&, id, lo, hi] {
            std::int64_t running = 0;
            for (int i = lo; i < hi; ++i) {
                running += values[i];
                prefix[i] = running;
            }
            block_sum[id] = running;
        });
    }
    pool.clear();

    std::vector<std::int64_t> offset(workers, 0);
    for (int id = 1; id < workers; ++id) {
        offset[id] = offset[id - 1] + block_sum[id - 1];
    }

    for (int id = 0; id < workers; ++id) {
        const int lo = id * n / workers;
        const int hi = (id + 1) * n / workers;
        pool.emplace_back([&, id, lo, hi] {
            for (int i = lo; i < hi; ++i) prefix[i] += offset[id];
        });
    }
    pool.clear();

    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << prefix[i];
    }
    std::cout << '\n';
    return 0;
}
