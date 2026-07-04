// Exercise A8.4 - The Packet Batches
// Session: A8, exercise 4 of 10
// Theme: chapter 04 (binary search on the answer)
// Tags: chapter-04, binary-search, greedy
//
// MODEL: split an ordered sequence into at most k contiguous groups
//   minimizing the maximum group sum.
// MATH: a candidate limit X is feasible iff the greedy scan can
//   pack the sequence into at most k batches without any batch sum
//   exceeding X.
// PROOF: exchange argument. For a fixed X, delaying each cut as far
//   as possible cannot increase the number of batches needed, so the
//   greedy count is minimal. Feasibility is monotone in X.
// COMPLEXITY: O(n log sum) time and O(1) extra memory.
// TYPES: total weight can reach 2e14, so int64_t is used.
// ALTERNATIVES: DP over cuts is too slow at n=2e5.
// EDGE CASES: k=1; k=n; one very large packet.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o batches a8-04-packet-batches.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febatches.exe a8-04-packet-batches.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

bool feasible(const std::vector<std::int64_t>& w, int k, std::int64_t limit) {
    int batches = 1;
    std::int64_t sum = 0;
    for (const std::int64_t x : w) {
        if (sum + x <= limit) {
            sum += x;
        } else {
            ++batches;
            sum = x;
        }
    }
    return batches <= k;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::vector<std::int64_t> w(n);
    std::int64_t low = 0;
    std::int64_t high = 0;
    for (auto& x : w) {
        std::cin >> x;
        low = std::max(low, x);
        high += x;
    }
    while (low < high) {
        const std::int64_t mid = low + (high - low) / 2;
        if (feasible(w, k, mid)) high = mid;
        else low = mid + 1;
    }
    std::print("{}\n", low);
    return 0;
}
