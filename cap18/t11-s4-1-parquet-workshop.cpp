// Exercise T11.5 - The Parquet Workshop
// Session: T11.S4, exercise 1 of 3 (2 hours for all three)
// Theme: chapter 11 (counting DP, linear recurrence by last column)
// Tags: chapter-11, dynamic-programming, modular-arithmetic
//
// MODEL: tilings of a 2 x n board with 1x2 dominoes (both
//   orientations) and 2x2 squares; count modulo p.
// MATH: condition on what covers the last column. A vertical plank:
//   leaves 2 x (n-1). Two horizontal planks stacked: leave
//   2 x (n-2). A 2x2 inlay: leaves 2 x (n-2). Hence
//   f(n) = f(n-1) + 2 f(n-2), f(0) = 1 (empty product), f(1) = 1.
// PROOF: the case split is exhaustive and exclusive - the cell in
//   the top-right corner is covered by exactly one of the three
//   piece placements, and each placement leaves a clean prefix
//   board (no jagged boundary is possible with these pieces on
//   height 2). Distinct cases give distinct tilings; induction on n
//   does the rest.
// COMPLEXITY: O(n) time, O(1) memory with two rolling variables.
//   (Matrix power would give O(log n) - see ALTERNATIVES.)
// TYPES: residues < 1e9+7; the sum f(n-1) + 2 f(n-2) < 3e9
//   overflows int32 BEFORE the reduction - long long, or reduce
//   between the two additions. The code uses long long.
// ALTERNATIVES: 2x2 matrix exponentiation answers in O(log n) and
//   wins when n reaches 1e18 or queries pile up; at n <= 1e6 a
//   linear loop is shorter and equally instant. Bitmask-profile DP
//   (the chapter 14 tool) generalizes to height 3+ but is
//   anachronistic here and overkill for height 2.
// EDGE CASES: n = 1 (single vertical plank: 1); n = 2 (vertical
//   pair, horizontal pair, inlay: 3); n = 3 (5, the example);
//   n = 1e6 (loop, no table needed).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o parquet t11-s4-1-parquet-workshop.cpp

#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n = 0;
    std::cin >> n;

    constexpr long long kMod = 1'000'000'007;
    long long two_back = 1;  // f(0)
    long long one_back = 1;  // f(1)
    for (long long i = 2; i <= n; ++i) {
        const long long current = (one_back + 2 * two_back) % kMod;
        two_back = one_back;
        one_back = current;
    }
    std::print("{}\n", one_back);
    return 0;
}
