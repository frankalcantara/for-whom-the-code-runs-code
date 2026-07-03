// Exercise T12.3 - The Typesetter's Tray
// Session: T12.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 12 (anagram windows, sliding frequency counts)
// Tags: chapter-12, sliding-window
//
// MODEL: count windows of t of length m = |p| whose letter
//   multiset equals that of p.
// MATH: maintain diff[c] = (count of c in the window) - (count of
//   c in p), and matched = number of letters with diff[c] = 0.
//   A window is an anagram iff matched = 26. Sliding one position
//   changes two letters; each change updates one diff cell and
//   adjusts matched by checking the cell against zero before and
//   after. No window is ever rescanned.
// PROOF: multiset equality is exactly diff = 0 in all 26 cells;
//   matched counts the zero cells, and the two single-cell updates
//   per slide keep it exact (a cell leaves/enters zero only at the
//   moment its counter crosses, which the before/after check
//   catches). Induction over window positions.
// COMPLEXITY: O(|t| + |p| + 26) time, O(26) extra memory.
// TYPES: counts fit int (<= 1e6); answer fits int but is long long
//   out of habit. Plain arrays over std::map - the alphabet is
//   fixed and tiny, locality wins (chapter 1's lesson applied).
// ALTERNATIVES: compare sorted windows - O(n m log m), dead;
//   compare full 26-counters per window - O(26 n), survives but
//   does 26x the work the matched counter avoids; polynomial
//   multiset hashing - works, probabilistic, no reason here.
// EDGE CASES: |p| = |t| (one window); |p| = 1 (counts one letter);
//   p with repeated letters (multiplicities, not sets - the diff
//   counter handles it by construction); no match anywhere (0);
//   alternating overlap chains (the example).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o tray t12-s3-1-typesetters-tray.cpp

#include <array>
#include <iostream>
#include <print>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string t;
    std::string p;
    std::cin >> t >> p;

    const std::size_t n = t.size();
    const std::size_t m = p.size();

    std::array<int, 26> diff{};  // window count minus pattern count
    int matched = 26;
    const auto adjust = [&](char ch, int delta) {
        const std::size_t c = static_cast<std::size_t>(ch - 'a');
        if (diff[c] == 0) --matched;
        diff[c] += delta;
        if (diff[c] == 0) ++matched;
    };

    for (const char ch : p) adjust(ch, -1);
    for (std::size_t i = 0; i < m; ++i) adjust(t[i], +1);

    long long count = (matched == 26) ? 1 : 0;
    for (std::size_t i = m; i < n; ++i) {
        adjust(t[i], +1);
        adjust(t[i - m], -1);
        if (matched == 26) ++count;
    }
    std::print("{}\n", count);
    return 0;
}
