// Exercise T05.5 - The Riverbed Survey
// Session: T05.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 05 (sparse table; static RMQ with O(1) queries)
// Tags: chapter-05
//
// MODEL: static range-minimum queries over an immutable array. The
//   statement promises no updates ("not to be measured again"), which is
//   the precondition the sparse table charges for its O(1) answers.
// MATH: table[j][i] = min of the block of length 2^j starting at i, built
//   by table[j][i] = min(table[j-1][i], table[j-1][i + 2^(j-1)]). A query
//   [l, r] takes j = floor(log2(r - l + 1)) and overlaps two blocks:
//   min(table[j][l], table[j][r - 2^j + 1]).
// PROOF: min is idempotent (min(x, x) = x), so the two blocks may overlap
//   without corrupting the answer; together they cover [l, r] because
//   2 * 2^j >= r - l + 1 by choice of j. Build correctness is induction
//   on j: each block of length 2^j is the union of its two halves.
// COMPLEXITY: O(n log n) build time and memory (~2e5 * 18 ints = 14 MB),
//   O(1) per query: O(n log n + q) total, which is what q up to 1e6
//   demands. A segment tree answers in O(log n) per query: 1e6 * 18 vs
//   1e6 operations, still passable, but the table is both faster and
//   simpler when there are no updates.
// TYPES: depths fit in int (<= 1e9 < 2^31 - 1) and min never grows, so
//   the whole table is int; the only widening is bit_width over the
//   unsigned length. Indices fit in int as well.
// ALTERNATIVES: segment tree (updates, not needed); sqrt decomposition
//   (O(sqrt n) per query, no reason); offline sorting by length is just
//   the sparse table rediscovered slowly. With 1e6 queries, the fast
//   parser from chapter 03 would also be a legitimate companion choice.
// EDGE CASES: l = r (single reach, j = 0); the whole river (the two
//   blocks overlap maximally); n = 1; repeated minima; depth 0.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o riverbed t05-s4-1-riverbed-survey.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feriverbed.exe t05-s4-1-riverbed-survey.cpp

#include <algorithm>
#include <bit>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    int levels = std::bit_width(static_cast<unsigned>(n));
    std::vector<std::vector<int>> table(levels, std::vector<int>(n));
    for (auto& d : table[0]) std::cin >> d;

    for (int j = 1; j < levels; ++j) {
        int half = 1 << (j - 1);
        for (int i = 0; i + (1 << j) <= n; ++i) {
            table[j][i] = std::min(table[j - 1][i], table[j - 1][i + half]);
        }
    }

    while (q-- > 0) {
        int l = 0;
        int r = 0;
        std::cin >> l >> r;
        --l;
        --r;
        int j = std::bit_width(static_cast<unsigned>(r - l + 1)) - 1;
        int ans = std::min(table[j][l], table[j][r - (1 << j) + 1]);
        std::print("{}\n", ans);
    }
    return 0;
}
