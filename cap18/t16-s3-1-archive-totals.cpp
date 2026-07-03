// Exercise T16.3 - The Archive Totals
// Session: T16.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 16 (std::async and futures)
// Tags: chapter-16
//
// MODEL: sum an array by splitting it into independent contiguous
//   blocks whose results are returned through futures.
// MATH: total sum equals the sum of block sums because the blocks
//   partition the index range. Each async task owns its range and
//   returns one int64_t partial result.
// PROOF: partition argument. Every index belongs to exactly one
//   block, so adding all future results adds every value once. The
//   future get operations synchronize with task completion and
//   retrieve each partial result exactly once.
// COMPLEXITY: O(n) total work and O(p) futures. Task overhead means
//   this is a technique to measure, not a blanket promise of speed.
// TYPES: the sum can reach 2e15 in magnitude, so int64_t is used.
// ALTERNATIVES: std::reduce with an execution policy is shorter
//   when the implementation supports it well; a sequential loop is
//   often best for small inputs.
// EDGE CASES: p > n; negative values; one value; cancellation is not
//   involved because every block is needed.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o totals t16-s3-1-archive-totals.cpp

#include <algorithm>
#include <cstdint>
#include <future>
#include <iostream>
#include <numeric>
#include <print>
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
    std::vector<std::future<std::int64_t>> futures;
    futures.reserve(static_cast<std::size_t>(workers));
    for (int id = 0; id < workers; ++id) {
        const int lo = id * n / workers;
        const int hi = (id + 1) * n / workers;
        futures.push_back(std::async(std::launch::async, [&, lo, hi] {
            return std::accumulate(values.begin() + lo, values.begin() + hi,
                                   std::int64_t{0});
        }));
    }

    std::int64_t total = 0;
    for (auto& future : futures) total += future.get();
    std::print("{}\n", total);
    return 0;
}
