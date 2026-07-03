// Exercise A2.4 - The Chandler's Crates
// Session: A2, exercise 4 of 4 (5 hours for the evaluation; integration)
// Theme: chapters 02 + 04 (greedy by exchange, prefix sums over the sorted
//   order, binary search per query)
// Tags: chapter-02, chapter-04, binary-search, sorting, greedy, prefix-sum
//
// MODEL: for each budget b, maximize the number of crates whose total
//   price is <= b.
// MATH: three tools compose. (1) Greedy: to maximize the COUNT, buy
//   cheapest first. (2) Prefix sums over the sorted prices turn "cost of
//   the j cheapest crates" into p[j], a non-decreasing array. (3) The
//   answer for budget b is the largest j with p[j] <= b: the boundary
//   upper_bound finds in O(log n).
// PROOF: (greedy, exchange argument) take any valid purchase of m crates;
//   replace its most expensive crate by the cheapest unused cheaper-or-
//   equal one. Repeating yields the m cheapest crates without raising the
//   total, so if any m crates fit the budget, the m cheapest do; hence
//   maximizing j with p[j] <= b is exactly the optimum. (boundary)
//   p is non-decreasing because prices are non-negative, so upper_bound's
//   precondition holds and the element before the boundary is the largest
//   prefix within budget.
// COMPLEXITY: O(n log n) sort + O(n) prefixes + O(log n) per budget:
//   O((n + q) log n) total, O(n) memory.
// TYPES: total price reaches 2e5 * 1e9 = 2e14, and budgets reach 1e18:
//   the prefix array and budgets are long long; the printed count is an
//   iterator distance, fitting comfortably in long long. Comparing a
//   budget of 1e18 with int prefixes would be the silent disaster.
// ALTERNATIVES: per-budget greedy scanning is O(nq) = 4e10; sorting the
//   budgets and sweeping once is O((n + q) log q) offline and a fine
//   answer too; the per-query logarithm keeps the program online and
//   stateless between monasteries, matching "every monastery shops the
//   same full catalogue".
// EDGE CASES: budget 0 (zero crates); budget below the cheapest crate;
//   budget covering everything; equal prices; q repeating one budget.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o chandler a2-04-chandler-crates.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> price(n);
    for (auto& p : price) std::cin >> p;
    std::ranges::sort(price);

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + price[i];

    while (q-- > 0) {
        long long budget = 0;
        std::cin >> budget;
        auto it = std::ranges::upper_bound(prefix, budget);
        std::print("{}\n", (it - prefix.begin()) - 1);
    }
    return 0;
}
