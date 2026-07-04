// Exercise A8.2 - The Counter Hall
// Session: A8, exercise 2 of 10
// Theme: chapter 16 (independent workers and atomic reduction)
// Tags: chapter-16, parallelism
//
// MODEL: count values satisfying a threshold predicate.
// MATH: split the array into disjoint blocks; the answer is the sum
//   of block counts.
// PROOF: partition argument. Each index belongs to exactly one
//   block, and atomic fetch_add prevents lost updates when workers
//   publish local counts.
// COMPLEXITY: O(n) work and O(p) worker objects.
// TYPES: the answer fits int64_t. Input values fit int.
// ALTERNATIVES: sequential counting is simpler for small n; a mutex
//   is unnecessary for one shared counter.
// EDGE CASES: p>n; all values active; no value active.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -pthread -o counter a8-02-counter-hall.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecounter.exe a8-02-counter-hall.cpp

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <print>
#include <thread>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int workers = 0;
    int threshold = 0;
    std::cin >> n >> workers >> threshold;
    std::vector<int> values(n);
    for (int& value : values) std::cin >> value;

    workers = std::min(workers, n);
    std::atomic<std::int64_t> answer{0};
    std::vector<std::jthread> pool;
    for (int id = 0; id < workers; ++id) {
        const int lo = id * n / workers;
        const int hi = (id + 1) * n / workers;
        pool.emplace_back([&, lo, hi] {
            std::int64_t local = 0;
            for (int i = lo; i < hi; ++i) {
                if (values[i] >= threshold) ++local;
            }
            answer.fetch_add(local, std::memory_order_relaxed);
        });
    }
    pool.clear();
    std::print("{}\n", answer.load(std::memory_order_relaxed));
    return 0;
}
