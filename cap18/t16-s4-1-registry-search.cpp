// Exercise T16.5 - The Registry Search
// Session: T16.S4, exercise 1 of 3 (2 hours for all three)
// Theme: chapter 16 (jthread and cooperative cancellation)
// Tags: chapter-16, parallelism
//
// MODEL: search independent ranges for one target. Once any worker
//   finds the value, remaining workers may stop early.
// MATH: the ranges form a partition of [0, n). The answer is FOUND
//   iff at least one range contains the target. A shared stop_source
//   broadcasts that further searching is unnecessary after success.
// PROOF: partition argument. If the target exists, it lies in some
//   worker's range; that worker sets found and requests stop unless
//   it was already stopped after another success. If no worker finds
//   the target, every range has been exhausted and found remains
//   false. Stop requests can only skip work after a success, so they
//   cannot turn a true answer into false.
// COMPLEXITY: O(n) worst-case work and O(p) workers. Best case may
//   stop earlier after a found target.
// TYPES: values fit int64_t. The shared found flag is atomic because
//   several workers may discover the target concurrently.
// ALTERNATIVES: a sequential scan is simpler; forced thread
//   termination would be unsafe and unnecessary.
// EDGE CASES: n = 0; target absent; target appears many times; p >
//   n; target at the first or last position.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -pthread -o search t16-s4-1-registry-search.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesearch.exe t16-s4-1-registry-search.cpp

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <print>
#include <stop_token>
#include <thread>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int workers = 0;
    std::int64_t target = 0;
    std::cin >> n >> workers >> target;
    std::vector<std::int64_t> values(n);
    for (std::int64_t& value : values) std::cin >> value;

    if (n == 0) {
        std::print("MISSING\n");
        return 0;
    }

    workers = std::min(workers, n);
    std::stop_source stop_source;
    std::atomic<bool> found{false};
    std::vector<std::jthread> pool;
    pool.reserve(static_cast<std::size_t>(workers));
    for (int id = 0; id < workers; ++id) {
        const int lo = id * n / workers;
        const int hi = (id + 1) * n / workers;
        pool.emplace_back([&, lo, hi](std::stop_token) {
            const std::stop_token token = stop_source.get_token();
            for (int i = lo; i < hi && !token.stop_requested(); ++i) {
                if (values[i] == target) {
                    found.store(true, std::memory_order_relaxed);
                    stop_source.request_stop();
                    break;
                }
            }
        });
    }

    pool.clear();
    std::print("{}\n", found.load(std::memory_order_relaxed) ? "FOUND"
                                                            : "MISSING");
    return 0;
}
