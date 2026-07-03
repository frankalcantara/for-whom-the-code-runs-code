// Exercise T11.6 - The Night Patrols
// Session: T11.S4, exercise 2 of 3 (2 hours for all three)
// Theme: chapter 11 (Stirling numbers of the second kind)
// Tags: chapter-11, dynamic-programming
//
// MODEL: partitions of an n-set into exactly k non-empty unlabeled
//   blocks - the Stirling number of the second kind S(n, k).
// MATH: recurrence on the last volunteer: either volunteer n forms
//   a patrol alone (S(n-1, k-1) plans for the rest) or joins one of
//   the k patrols the others already form (k * S(n-1, k)):
//   S(n, k) = S(n-1, k-1) + k * S(n-1, k),
//   S(0, 0) = 1, S(t, 0) = 0 for t > 0, S(t, j) = 0 for j > t.
// PROOF: the two cases partition the set of plans by whether
//   volunteer n is alone; the maps to smaller instances are
//   bijections (remove volunteer n; in the second case remember
//   which block absorbed them - k choices, blocks distinguished by
//   their other members, all non-empty since n was not alone).
// COMPLEXITY: O(nk) time, O(k) memory with one rolling row updated
//   right to left... careful: S(n, j) needs S(n-1, j-1) and
//   S(n-1, j), so an in-place row must be updated from HIGH j to
//   low j (the code does), or use two rows. 5000 * 5000 / 2 ~ 12.5M
//   cell updates: well under a second.
// TYPES: residues and k * S(...) products < 5e3 * 1e9 < 1e13:
//   long long. Row indices int-sized.
// ALTERNATIVES: the closed form
//   S(n, k) = (1/k!) sum_j (-1)^j C(k, j) (k-j)^n
//   is O(k log n) for ONE pair - it would beat the DP here, and is
//   worth knowing; the DP wins when a whole table is needed and as
//   the proof-generating form. Bell numbers sum S(n, k) over k -
//   not asked.
// EDGE CASES: k = n (everyone alone: 1); k = 1 (one patrol: 1);
//   n = 1 (1); k > n impossible by constraints; n = k = 5000
//   (diagonal: 1, exercises the table bounds).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o patrols t11-s4-2-night-patrols.cpp

#include <cstddef>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t k = 0;
    std::cin >> n >> k;

    constexpr long long kMod = 1'000'000'007;
    // row[j] holds S(t, j) for the current t; updated in place from
    // high j to low j so row[j-1] is still the previous generation.
    std::vector<long long> row(k + 1, 0);
    row[0] = 1;  // S(0, 0)
    for (std::size_t t = 1; t <= n; ++t) {
        const std::size_t high = std::min(t, k);
        for (std::size_t j = high; j >= 1; --j) {
            row[j] = (row[j - 1] + static_cast<long long>(j) * row[j]) % kMod;
        }
        row[0] = 0;  // S(t, 0) = 0 for t > 0
    }
    std::print("{}\n", row[k]);
    return 0;
}
