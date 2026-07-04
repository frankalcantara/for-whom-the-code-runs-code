// Exercise T01.1 - The Cistern Keeper
// Session: T01.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 01 (linear scan, integer width chosen from the constraints)
// Tags: chapter-01, prefix-sum
//
// MODEL: given w_1..w_n and a threshold C, find the smallest k such that
//   S_k = w_1 + ... + w_k > C, or report -1 when no prefix exceeds C.
// MATH: the inflows satisfy w_i >= 0, so S_k is non-decreasing in k. The set
//   of days whose prefix sum exceeds C is therefore a suffix of 1..n, and
//   the first day found by a left-to-right scan is the minimum of that set.
// PROOF: loop invariant. Before reading w_k, `stored` equals S_{k-1} and no
//   prefix among S_1..S_{k-1} exceeds C. Reading w_k restores the invariant
//   or stops at the first violating day; induction on k gives correctness.
// COMPLEXITY: O(n) time, O(1) memory beyond the input stream.
// TYPES: S_n can reach 1e6 * 1e12 = 1e18 < 2^63 - 1, so long long is
//   sufficient and necessary; int overflows on the first large inflow.
//   C itself reaches 1e18, so the comparison must also happen in 64 bits.
// ALTERNATIVES: storing all inflows or building a prefix array costs O(n)
//   memory and buys nothing; only the running sum is ever consulted.
// EDGE CASES: flood on day 1 (w_1 > C); a season that never floods (-1);
//   zero inflows (the sum must not move); stored == C exactly (not a flood,
//   the contract says strictly above).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o cistern t01-s1-1-cistern-keeper.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecistern.exe t01-s1-1-cistern-keeper.cpp

#include <cstdint>
#include <iostream>
#include <print>

int main() {
    int n = 0;
    long long mark = 0;
    std::cin >> n >> mark;

    long long stored = 0;
    int flood_day = -1;
    for (int day = 1; day <= n; ++day) {
        long long inflow = 0;
        std::cin >> inflow;
        stored += inflow;
        if (flood_day == -1 && stored > mark) flood_day = day;
    }

    std::print("{}\n", flood_day);
    return 0;
}
