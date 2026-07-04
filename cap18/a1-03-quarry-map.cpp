// Exercise A1.3 - The Quarry Map
// Session: A1, exercise 3 of 3 (4 hours for the evaluation; integration)
// Theme: chapters 01 + 02 (2D prefix sums, window enumeration, overflow,
//   deterministic tie rules)
// Tags: chapter-01, chapter-02, prefix-sum, sliding-window, brute-force
//
// MODEL: maximize the sum of a k x k block over all placements; report the
//   lexicographically smallest (top row, left column) among maxima.
// MATH: with the bordered 2D prefix table P,
//     block(i, j) = P[i+k-1][j+k-1] - P[i-1][j+k-1]
//                 - P[i+k-1][j-1] + P[i-1][j-1]
//   in 1-based coordinates, by the same inclusion-exclusion as any
//   rectangle query. There are (r-k+1)(c-k+1) placements; each costs O(1)
//   after the O(rc) build.
// PROOF: the prefix identity is exact (induction over cells); scanning
//   placements in increasing row, then increasing column, and replacing
//   the champion only on a strictly larger total makes the first champion
//   of every value the smallest (row, column) pair, which is the contract's
//   tie rule. Every placement is examined, so the champion is the global
//   maximum.
// COMPLEXITY: O(rc) build + O((r-k+1)(c-k+1)) scan = O(rc) total; at the
//   limits, ~1e6 placements. O(rc) memory for the prefix table.
// TYPES: a block can total 1000 * 1000 * 1e9 = 1e15: long long throughout
//   the table. Indices fit in int.
// ALTERNATIVES: summing each block directly costs O(rc * k^2), up to 1e12
//   visits; row-prefix plus column sliding achieves O(rc) with O(c) extra
//   memory and is the move when the full table does not fit. Here it fits.
// EDGE CASES: k == r == c (one placement); k == 1 (the best single cell,
//   first occurrence wins); a tie between two blocks in the same row
//   (smaller column must win); all-zero grid (answer 1 1 0).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o quarry a1-03-quarry-map.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fequarry.exe a1-03-quarry-map.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int r = 0;
    int c = 0;
    int k = 0;
    std::cin >> r >> c >> k;

    std::vector<std::vector<long long>> p(
        r + 1, std::vector<long long>(c + 1, 0));

    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= c; ++j) {
            long long quality = 0;
            std::cin >> quality;
            p[i][j] = quality + p[i - 1][j] + p[i][j - 1] - p[i - 1][j - 1];
        }

    int best_row = 1;
    int best_col = 1;
    long long best_total = -1;

    for (int i = 1; i + k - 1 <= r; ++i)
        for (int j = 1; j + k - 1 <= c; ++j) {
            long long total = p[i + k - 1][j + k - 1] - p[i - 1][j + k - 1]
                            - p[i + k - 1][j - 1] + p[i - 1][j - 1];
            if (total > best_total) {
                best_total = total;
                best_row = i;
                best_col = j;
            }
        }

    std::print("{} {} {}\n", best_row, best_col, best_total);
    return 0;
}
