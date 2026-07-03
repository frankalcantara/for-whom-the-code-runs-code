// Exercise A5.6 - The Purser's Split
// Session: A5, exercise 6 of 7 (6 hours for the evaluation)
// Theme: chapter 02 revision (running prefix sums in one pass), with
//   chapter 01's width arithmetic settling the type
// Tags: chapter-02, chapter-01, prefix-sum, brute-force
//
// MODEL: count the indices p in [1, n) where prefix(p) equals
//   suffix(p+1). With S the grand total, the condition
//   prefix(p) = S - prefix(p) rearranges to 2 * prefix(p) = S: one
//   running sum, one comparison per day, no second array.
// MATH: prefix(p) accumulates left to right; S is known after one
//   read of the input (which must be stored or summed first - the
//   comparison needs S before day 1 can be judged). Negative entries
//   are the whole game: with all-positive entries the prefix is
//   strictly increasing and at most one split can match, but storms
//   make the prefix wander, so any number of days can satisfy the
//   equation - the example has two.
// PROOF: the rearrangement is an equivalence (no division, so no
//   parity trap: if S is odd, 2 * prefix(p) = S simply never holds,
//   which is the correct answer to a question about equal integer
//   halves). Each p is tested independently against an exact
//   condition; the count is therefore exact by enumeration.
// COMPLEXITY: O(n) time, O(n) memory for the stored entries (or O(1)
//   with two passes over the input; at n = 1e6 the vector costs 8 MB
//   and reads once - chapter 2's advice is to take the vector and
//   stop being clever). At this input size, chapter 3's fast-I/O
//   reflexes (sync_with_stdio(false), tie(nullptr)) are assumed, not
//   optional.
// TYPES: prefixes reach n * max|d| = 1e6 * 1e9 = 1e15 in magnitude,
//   and the comparison doubles that to 2e15: int64 territory three
//   times over before the first line of code is written. int
//   overflows at day ~2147 of a bad voyage.
// ALTERNATIVES: materializing a full prefix array (harmless, just
//   unnecessary); hashing prefix values (chapter 06) answers a harder
//   query - "how many PAIRS of equal prefixes" - that nobody asked;
//   recomputing the suffix sum per p is the O(n^2) tax on not
//   noticing the rearrangement.
// EDGE CASES: n = 2 (one possible split); all entries zero (every
//   split works: answer n - 1); odd grand total (answer 0); equal
//   halves only at the first or last valid p; entries summing to
//   zero overall (splits where prefix(p) = 0).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o split a5-06-pursers-split.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<std::int64_t> day(static_cast<std::size_t>(n));
    std::int64_t grand_total = 0;
    for (auto& d : day) {
        std::cin >> d;
        grand_total += d;
    }

    std::int64_t prefix = 0;
    std::int64_t splits = 0;
    for (int p = 1; p < n; ++p) {  // p = days given to the first partner
        prefix += day[static_cast<std::size_t>(p - 1)];
        if (2 * prefix == grand_total) {
            ++splits;
        }
    }

    std::println("{}", splits);
    return 0;
}
