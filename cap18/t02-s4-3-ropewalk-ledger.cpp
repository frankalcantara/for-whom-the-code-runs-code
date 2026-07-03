// Exercise T02.7 - The Ropewalk Ledger
// Session: T02.S4, exercise 3 of 3 (2 hours for the three; this one resists)
// Theme: chapter 02 (prefix sums meet hashing: equal prefixes, zero ranges)
// Tags: chapter-02, prefix-sum, number-theory
//
// MODEL: count pairs (i, j), i <= j, with a_i + ... + a_j = 0.
// MATH: let P_0 = 0 and P_k = a_1 + ... + a_k. Then
//     a_i + ... + a_j = P_j - P_{i-1},
//   so a stretch sums to zero exactly when P_j = P_{i-1}. The question
//   "how many zero stretches" becomes "how many unordered pairs of equal
//   values among P_0..P_n". If a prefix value occurs f times, it
//   contributes f(f-1)/2 pairs (each unordered pair of occurrences is one
//   stretch, the earlier occurrence acting as i-1 and the later as j).
// PROOF: bijection. The map (i, j) -> {P_{i-1}, P_j as positions} sends
//   each zero stretch to one unordered pair of equal-prefix positions, and
//   any such pair (s < t) maps back to the stretch (s+1 .. t); the two maps
//   are inverse. Counting pairs per value with f(f-1)/2 is the Chapter 1
//   handshake count applied to each group.
// COMPLEXITY: expected O(n) with a hash map of prefix frequencies built in
//   one pass; O(d) memory for d distinct prefix values.
// TYPES: prefixes reach |2e14| (2e5 days of |1e9|): long long keys. The
//   answer can reach n(n+1)/2 ~ 2e10 for an all-zero ledger: long long.
//   This is the quiet trap of the exercise; int loses the answer, not the
//   prefixes.
// ALTERNATIVES: trying all stretches with prefix subtraction is O(n^2),
//   2e10 visits; sorting the prefix values and counting equal runs is
//   O(n log n) and deterministic, a respectable second choice. The hash
//   map matches the sparse, orderless key space.
// EDGE CASES: an all-zero ledger (every stretch counts, maximal answer);
//   no zero stretch at all; a single day of value 0; stretches starting on
//   day 1 (this is why P_0 = 0 must be counted as an occurrence).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o ropewalk t02-s4-3-ropewalk-ledger.cpp

#include <iostream>
#include <print>
#include <unordered_map>

int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<long long, long long> seen;
    seen.reserve(n + 1);
    seen[0] = 1;  // P_0: the empty prefix, anchor of stretches from day 1

    long long prefix = 0;
    long long serene = 0;
    for (int day = 1; day <= n; ++day) {
        long long amount = 0;
        std::cin >> amount;
        prefix += amount;
        serene += seen[prefix];  // pairs with every earlier equal prefix
        ++seen[prefix];
    }

    std::print("{}\n", serene);
    return 0;
}
