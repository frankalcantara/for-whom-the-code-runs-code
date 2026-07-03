// Exercise T08.2 - The Peddler's Pack
// Session: T08.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 08 (0/1 knapsack; rolling array)
// Tags: chapter-08, dynamic-programming, knapsack
//
// MODEL: 0/1 knapsack. Each ware is a take-it-or-leave-it decision;
//   the state that makes decisions independent is "best profit using a
//   prefix of the wares within capacity c".
// MATH: dp[i][c] = max(dp[i-1][c], dp[i-1][c - w_i] + p_i), the second
//   option only when c >= w_i; dp[0][*] = 0. Row i depends only on row
//   i-1 at indices <= c, so a single array updated with c descending
//   from W to w_i implements the recurrence in place: descending order
//   guarantees dp[c - w_i] still holds the row i-1 value when row i is
//   written - ascending order would let ware i be taken twice.
// PROOF: induction on i: an optimal selection from the first i wares
//   within capacity c either omits ware i (value dp[i-1][c]) or takes
//   it (value dp[i-1][c - w_i] + p_i); both subcases are optimal by
//   the induction hypothesis, and the max over the only two cases is
//   exhaustive. The in-place subtlety above is the entire content of
//   the chapter's rolling-array lesson, met here in its native habitat.
// COMPLEXITY: O(nW) time = 1e7 cell updates at the limits, O(W)
//   memory = 8e5 bytes. The two-row version costs double the memory
//   for no benefit; the full table costs 100x.
// TYPES: profits are up to 1e9 each and up to 100 of them stack, so
//   totals reach 1e11: the dp array must hold long long. Weights and
//   capacity fit in int.
// ALTERNATIVES: meet in the middle handles n <= 40 with huge W (the
//   Ballast Stones road, T07.7); branch and bound is respectable when
//   W is astronomical; greedy by profit density solves the FRACTIONAL
//   knapsack only - with whole clocks it has no guarantee at all.
// EDGE CASES: a single ware heavier than... no, w_i <= W by contract,
//   so every ware fits alone; selection that fills W exactly; all
//   wares together within W (take all); n = 1; equal weights with
//   distinct profits (the dp must prefer the profit, not the order).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o pack t08-s2-1-peddlers-pack.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int capacity = 0;
    std::cin >> n >> capacity;

    std::vector<long long> dp(static_cast<std::size_t>(capacity) + 1, 0);
    for (int i = 0; i < n; ++i) {
        int w = 0;
        long long p = 0;
        std::cin >> w >> p;
        for (int c = capacity; c >= w; --c) {
            dp[static_cast<std::size_t>(c)] =
                std::max(dp[static_cast<std::size_t>(c)],
                         dp[static_cast<std::size_t>(c - w)] + p);
        }
    }
    std::print("{}\n", dp[static_cast<std::size_t>(capacity)]);
    return 0;
}
