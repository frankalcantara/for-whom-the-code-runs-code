// Exercise A7.2 - The Patent Drawers
// Session: A7, exercise 2 of 9
// Theme: chapter 14 (bitmask DP for set cover)
// Tags: chapter-14, dynamic-programming, bitmask-dp, bitmask
//
// MODEL: choose the minimum number of subsets whose union is the
//   full set of n categories.
// MATH: dp[mask] is the fewest drawers needed to cover exactly the
//   categories in mask, or infinity if mask is not yet reachable.
//   For each drawer with coverage c, transition mask -> mask | c.
//   The full mask contains every category.
// PROOF: induction over processed drawers. Before any drawer, only
//   the empty union is reachable with cost 0. Each transition
//   either ignores the drawer or opens it and adds its categories.
//   Every subset of opened drawers is represented by the sequence
//   of these choices, and every DP transition corresponds to such a
//   choice. Therefore dp[full] is the optimum.
// COMPLEXITY: O(m * 2^n) time and O(2^n) memory. With n <= 20, the
//   table has about one million states.
// TYPES: masks fit uint32_t because n <= 20. The answer is at most
//   m <= 80, so int is ample.
// ALTERNATIVES: greedy set cover is only approximate; backtracking
//   can be faster on friendly inputs but has less predictable
//   behavior than the explicit subset DP at this size.
// EDGE CASES: a drawer covering no new category; impossible
//   coverage; n = 1; duplicate categories are excluded by the
//   statement; several drawers with identical coverage.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o drawers a7-02-patent-drawers.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<std::uint32_t> cover(m, 0);
    for (int i = 0; i < m; ++i) {
        int count = 0;
        std::cin >> count;
        for (int j = 0; j < count; ++j) {
            int category = 0;
            std::cin >> category;
            cover[i] |= std::uint32_t{1} << (category - 1);
        }
    }

    const std::uint32_t full = (std::uint32_t{1} << n) - 1;
    const int infinity = m + 1;
    std::vector<int> dp(full + 1, infinity);
    dp[0] = 0;
    for (const std::uint32_t drawer : cover) {
        for (std::uint32_t mask = 0; mask <= full; ++mask) {
            if (dp[mask] == infinity) continue;
            const std::uint32_t next = mask | drawer;
            dp[next] = std::min(dp[next], dp[mask] + 1);
        }
    }

    std::print("{}\n", dp[full] == infinity ? -1 : dp[full]);
    return 0;
}
