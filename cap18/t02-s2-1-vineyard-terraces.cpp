// Exercise T02.2 - The Vineyard Terraces
// Session: T02.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 02 (2D prefix sums, inclusion-exclusion bookkeeping)
// Tags: chapter-02, prefix-sum, dynamic-programming
//
// MODEL: an r x c grid and q rectangle-sum queries. Build
//   P[i][j] = sum of the i x j upper-left subgrid, with a zero row and
//   column as border. Then, for 1-based inclusive corners,
//     S = P[r2][c2] - P[r1-1][c2] - P[r2][c1-1] + P[r1-1][c1-1].
// MATH: the build recurrence is
//   P[i][j] = grid[i][j] + P[i-1][j] + P[i][j-1] - P[i-1][j-1];
//   the two big prefixes both contain the upper-left overlap, which was
//   therefore added twice and must be subtracted once. The query applies
//   the same inclusion-exclusion in the other direction: remove the strip
//   above, remove the strip to the left, restore their intersection,
//   removed twice.
// PROOF: induction over cells in row-major order establishes the build
//   invariant; the query identity is then pure set algebra on four
//   axis-aligned regions whose signed measures cancel outside the block.
// COMPLEXITY: O(rc) build, O(1) per query; O(rc) memory.
// TYPES: the whole grid can sum to 1000 * 1000 * 1e9 = 1e15; long long.
// ALTERNATIVES: per-query scanning costs O(rc) each, up to 2e11 visits
//   total; row-only prefixes give O(r) per query, 2e8 visits, borderline
//   where full 2D preprocessing is exact and cheap.
// EDGE CASES: 1x1 block; a block equal to the whole grid; blocks touching
//   the borders (the zero border row/column absorbs the index arithmetic);
//   all-zero yields.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o vineyard t02-s2-1-vineyard-terraces.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int r = 0;
    int c = 0;
    int q = 0;
    std::cin >> r >> c >> q;

    std::vector<std::vector<long long>> p(
        r + 1, std::vector<long long>(c + 1, 0));

    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= c; ++j) {
            long long yield = 0;
            std::cin >> yield;
            p[i][j] = yield + p[i - 1][j] + p[i][j - 1] - p[i - 1][j - 1];
        }

    while (q-- > 0) {
        int r1 = 0;
        int c1 = 0;
        int r2 = 0;
        int c2 = 0;
        std::cin >> r1 >> c1 >> r2 >> c2;
        long long total = p[r2][c2] - p[r1 - 1][c2]
                        - p[r2][c1 - 1] + p[r1 - 1][c1 - 1];
        std::print("{}\n", total);
    }
    return 0;
}
