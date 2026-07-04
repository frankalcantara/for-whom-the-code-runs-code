// Exercise T02.6 - The Salt Lagoon
// Session: T02.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 02 (flat matrix storage, index arithmetic, aggregation)
// Tags: chapter-02
//
// MODEL: for each anti-diagonal d = i + j of an r x c grid, accumulate the
//   total yield; report the smallest d with maximum total.
// MATH: d ranges over 0 .. r + c - 2, so there are r + c - 1 buckets.
//   Every cell belongs to exactly one bucket, so one pass over the grid in
//   reading order classifies all rc cells. No cell storage is needed at
//   all: the bucket index is computable while reading.
// PROOF: partition argument. The map (i, j) -> i + j partitions the cells;
//   adding each yield to its unique bucket makes bucket[d] the exact
//   anti-diagonal total. The final scan over buckets in increasing d keeps
//   the first maximum, which is the smallest-index tie rule.
// COMPLEXITY: O(rc) time, O(r + c) memory; the grid itself is never stored.
// TYPES: one anti-diagonal holds at most min(r, c) <= 2000 cells of up to
//   1e9, so a bucket reaches 2e12: long long. The bucket index fits in int.
// ALTERNATIVES: storing the grid (flat or nested) and walking diagonals
//   afterward costs O(rc) memory for no benefit; the nested-vector version
//   also adds one allocation per row. Reading-order accumulation is the
//   cheapest correct shape, and the lesson is recognizing that the matrix
//   was never needed, only its coordinates.
// EDGE CASES: single row or single column (every cell its own diagonal or
//   one diagonal per cell); all yields zero (answer is diagonal 0); ties
//   between diagonals (smallest index wins); maximum-size grid.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o lagoon t02-s4-2-salt-lagoon.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Felagoon.exe t02-s4-2-salt-lagoon.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int r = 0;
    int c = 0;
    std::cin >> r >> c;

    std::vector<long long> bucket(r + c - 1, 0);
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j) {
            long long yield = 0;
            std::cin >> yield;
            bucket[i + j] += yield;
        }

    int best_d = 0;
    for (int d = 1; d < static_cast<int>(bucket.size()); ++d)
        if (bucket[d] > bucket[best_d]) best_d = d;

    std::print("{} {}\n", best_d, bucket[best_d]);
    return 0;
}
