// Exercise A6.7 - The Fishmonger's Week
// Session: A6, exercise 7 of 8 (6 hours for the set)
// Theme: chapter 05 revision (Kadane's algorithm, non-empty
//   variant)
// Tags: chapter-05
//
// MODEL: maximum sum over non-empty contiguous subarrays.
// MATH: let best_end[i] = maximum sum of a subarray ending exactly
//   at i. Then best_end[i] = a[i] + max(best_end[i-1], 0): either
//   the best run ending at i-1 is worth extending, or i starts
//   fresh. The answer is max_i best_end[i]. One variable carries
//   best_end; another carries the running maximum, seeded with the
//   first element so the all-negative ledger still names its least
//   bad day.
// PROOF: induction on i - a subarray ending at i either is {a[i]}
//   alone or extends one ending at i-1, and extending is optimal
//   exactly when that one is positive; both cases are in the max.
//   Every non-empty subarray ends somewhere, so the outer max
//   covers them all.
// COMPLEXITY: O(n) time, O(1) memory beyond the input stream - the
//   array need not even be stored.
// TYPES: the sum reaches 1e6 * 1e9 = 1e15 in either direction:
//   long long for both accumulators; int for a_i would also poison
//   the additions through promotion games - everything is long
//   long from the read on.
// ALTERNATIVES: prefix sums with running minimum - same O(n), an
//   equally good answer (max over i of pref[i] - min pref before
//   i); divide and conquer O(n log n) - the segment-tree-friendly
//   form, overkill here; brute force O(n^2) = 1e12, dead.
// EDGE CASES: all days negative (the largest single element - the
//   non-empty contract forbids answering 0); n = 1; the maximum in
//   the middle flanked by losses; alternating signs (the example);
//   sums at +-1e15.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o week a6-07-fishmongers-week.cpp

#include <algorithm>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;

    long long best_end = 0;
    long long best = 0;
    for (std::size_t i = 0; i < n; ++i) {
        long long a = 0;
        std::cin >> a;
        if (i == 0) {
            best_end = a;
            best = a;
        } else {
            best_end = a + std::max(best_end, 0LL);
            best = std::max(best, best_end);
        }
    }
    std::print("{}\n", best);
    return 0;
}
