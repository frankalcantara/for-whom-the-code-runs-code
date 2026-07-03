// Exercise T01.3 - The Forge Trials
// Session: T01.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 01 (measurement discipline, exact integer comparison)
// Tags: chapter-01
//
// MODEL: discard t_1; let m = min(t_2..t_r); count the i >= 2 such that
//   t_i <= 1.1 * m.
// MATH: "at most ten percent slower" means t <= m * 11/10. Comparing in
//   floating point invites rounding at the boundary; multiplying both sides
//   by 10 gives the equivalent integer test
//     10 * t <= 11 * m,
//   which is exact. The minimum itself always passes: 10m <= 11m.
// PROOF: the two-pass argument is direct. The first pass computes m, which
//   is the true minimum of the retained multiset by the fold invariant
//   (after i values, `fastest` is their minimum). The second pass applies
//   the exact predicate to every retained value once.
// COMPLEXITY: O(r) time, O(r) memory for the stored measurements.
// TYPES: t_i <= 1e12, so 11 * m <= 1.1e13 and 10 * t <= 1e13; both fit
//   comfortably in long long but overflow int and unsigned int.
// ALTERNATIVES: a single pass keeping all candidates within the current
//   threshold is possible but needs re-filtering whenever the minimum
//   improves; with r <= 2e5, two clean passes are simpler and just as fast.
//   Floating point "works" until a judge case lands exactly on the border.
// EDGE CASES: r == 2 (one retained measurement, answer 1); all retained
//   values equal; the warm-up being the global minimum (it must still be
//   discarded); a value exactly 10% above the minimum (faithful).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o forge t01-s3-1-forge-trials.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int r = 0;
    std::cin >> r;

    std::vector<long long> t(r);
    for (auto& x : t) std::cin >> x;

    long long fastest = t[1];
    for (int i = 2; i < r; ++i)
        if (t[i] < fastest) fastest = t[i];

    int faithful = 0;
    for (int i = 1; i < r; ++i)
        if (10 * t[i] <= 11 * fastest) ++faithful;

    std::print("{}\n", faithful);
    return 0;
}
