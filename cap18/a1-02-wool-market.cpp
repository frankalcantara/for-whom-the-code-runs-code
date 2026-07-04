// Exercise A1.2 - The Wool Market
// Session: A1, exercise 2 of 3 (4 hours for the evaluation)
// Theme: chapter 02 (sort once as preprocessing, ordered queries by
//   upper_bound)
// Tags: chapter-02, binary-search, sorting
//
// MODEL: static multiset of weights; each query w asks for
//   |{ i : weight_i <= w }|.
// MATH: after sorting, the weights not greater than w form a prefix. The
//   first element strictly greater than w marks the end of that prefix, so
//   the answer is the index of that boundary. That boundary is exactly
//   what upper_bound returns.
// PROOF: sorting preserves the multiset (a permutation changes positions,
//   not membership), and in nondecreasing order every element before the
//   first > w is <= w while every element from it onward is > w. The
//   distance from the beginning to the boundary therefore counts exactly
//   the qualifying fleeces. upper_bound's contract requires sorted input;
//   that precondition is paid once, up front.
// COMPLEXITY: O(n log n) sort, O(log n) per query: O((n + q) log n) total,
//   against O(nq) = 4e10 for rescanning per buyer. O(n) memory.
// TYPES: weights reach 1e12, beyond int: long long elements. Counts fit in
//   int, but the printed value is the iterator distance, naturally a
//   signed 64-bit value; printing it as such avoids a narrowing cast that
//   would buy nothing.
// ALTERNATIVES: a counting array over the weight domain is 1e12 counters
//   (furniture, not an algorithm); answering offline by sorting queries
//   too is a classic two-pointer alternative at the same total cost, with
//   more moving parts than this contract needs.
// EDGE CASES: w smaller than every weight (answer 0); w >= the maximum
//   (answer n); w equal to a repeated weight (all copies count, which is
//   why upper_bound, not lower_bound); n = 1.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o wool a1-02-wool-market.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fewool.exe a1-02-wool-market.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> weight(n);
    for (auto& x : weight) std::cin >> x;
    std::ranges::sort(weight);

    while (q-- > 0) {
        long long limit = 0;
        std::cin >> limit;
        auto boundary = std::ranges::upper_bound(weight, limit);
        std::print("{}\n", boundary - weight.begin());
    }
    return 0;
}
