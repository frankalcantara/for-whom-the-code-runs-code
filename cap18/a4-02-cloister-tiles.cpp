// Exercise A4.2 - The Cloister Tiles
// Session: A4, exercise 2 of 6 (6 hours for the evaluation)
// Theme: chapter 08 (counting DP on a grid; rolling row)
// Tags: chapter-08, dynamic-programming, modular-arithmetic
//
// MODEL: count monotone lattice paths (east/south moves only) in a
//   grid with forbidden cells. Monotonicity makes the grid a DAG
//   processed row by row, which is what licenses a DP at all.
// MATH: ways[i][j] = ways[i-1][j] + ways[i][j-1] when tile (i, j) is
//   sound, 0 when it is under repair; ways[0][0] = 1 if sound. All
//   arithmetic mod p = 1e9+7. Row i needs only row i-1 and the entry
//   just written, so one row of c values, updated left to right,
//   suffices: ways[j] += ways[j-1] performs both reads correctly
//   (ways[j] still holds the row above, ways[j-1] is already current).
// PROOF: every east/south route arriving at (i, j) makes its last move
//   from the north or from the west, and the two classes are disjoint
//   and exhaustive, so the counts add - the bijection between routes
//   to (i, j) and routes to its two predecessors, by induction over
//   the row-major order. Forbidden tiles contribute 0 routes by
//   definition. Without obstacles the count collapses to the binomial
//   C(r + c - 2, r - 1) - worth knowing as a sanity check, useless
//   under repair tiles, which is precisely why the DP earns its keep.
// COMPLEXITY: O(rc) time (1e6 cell updates), O(c) memory with the
//   rolling row. Both fit the limits with two orders of magnitude to
//   spare; the exercise is about writing the in-place update without
//   fear.
// TYPES: counts are reduced mod 1e9+7 at every addition; the sum of
//   two reduced values stays below 2^31, so int would do, but long
//   long costs nothing and removes the conversation entirely.
// ALTERNATIVES: DFS with memoization is the same DP with recursion
//   stack risk at 1e6 cells; inclusion-exclusion over obstacles with
//   binomials handles SPARSE obstacles on astronomically large grids
//   (Chapter 11's combinatorics meeting Chapter 10's modular
//   binomials, neither needed at r, c <= 1000).
// EDGE CASES: start or goal under repair (0); 1 x 1 grid (1 if sound);
//   a full row of repairs (0 - no route crosses it); single row or
//   column with a gap (0) and without (1); the obstacle-free grid
//   (binomial check).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tiles a4-02-cloister-tiles.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetiles.exe a4-02-cloister-tiles.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int r = 0;
    int c = 0;
    std::cin >> r >> c;

    constexpr long long kMod = 1'000'000'007;
    std::vector<long long> ways(static_cast<std::size_t>(c), 0);

    for (int i = 0; i < r; ++i) {
        std::string row;
        std::cin >> row;
        for (int j = 0; j < c; ++j) {
            const std::size_t uj = static_cast<std::size_t>(j);
            if (row[uj] == '#') {
                ways[uj] = 0;
            } else if (i == 0 && j == 0) {
                ways[uj] = 1;
            } else {
                const long long from_west = (j > 0) ? ways[uj - 1] : 0;
                ways[uj] = (ways[uj] + from_west) % kMod;  // ways[uj] = from north
            }
        }
    }
    std::print("{}\n", ways[static_cast<std::size_t>(c) - 1]);
    return 0;
}
