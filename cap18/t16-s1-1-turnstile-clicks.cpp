// Exercise T16.1 - The Turnstile Clicks
// Session: T16.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 16 (jthread workers and an atomic counter)
// Tags: chapter-16, parallelism
//
// MODEL: count how many independent log entries are equal to one.
// MATH: split the index range into p contiguous blocks. Each worker
//   counts its own block privately, then adds that local count to a
//   shared atomic total. Since addition is associative, the order in
//   which blocks publish their counts does not affect the result.
// PROOF: partition argument. The blocks are disjoint and their union
//   is exactly [0, n). Each entry is examined once by exactly one
//   worker. The atomic fetch_add operation prevents lost updates
//   when local counts are combined, so the final total is the sum of
//   all block counts.
// COMPLEXITY: O(n) total work and O(p) extra memory for workers.
// TYPES: n <= 2e6, so the count fits int, but int64_t is used for
//   the atomic total and output.
// ALTERNATIVES: a sequential loop is simpler and often faster for
//   small n; a mutex-protected total would be heavier than an atomic
//   because one memory location is the only shared state.
// EDGE CASES: p > n; all zeros; all ones; n = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o clicks t16-s1-1-turnstile-clicks.cpp

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
    std::cin >> n >> workers;
    std::vector<int> readings(n);
    for (int& value : readings) std::cin >> value;

    workers = std::min(workers, n);
    std::atomic<std::int64_t> total{0};
    std::vector<std::jthread> pool;
    pool.reserve(static_cast<std::size_t>(workers));

    for (int id = 0; id < workers; ++id) {
        const int lo = id * n / workers;
        const int hi = (id + 1) * n / workers;
        pool.emplace_back([&, lo, hi] {
            std::int64_t local = 0;
            for (int i = lo; i < hi; ++i) {
                if (readings[i] == 1) ++local;
            }
            total.fetch_add(local, std::memory_order_relaxed);
        });
    }

    pool.clear();
    std::print("{}\n", total.load(std::memory_order_relaxed));
    return 0;
}
