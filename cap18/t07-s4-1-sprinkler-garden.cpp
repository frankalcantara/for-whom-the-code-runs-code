// Exercise T07.5 - The Sprinkler Garden
// Session: T07.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 07 (backtracking with bitmask state; N-Queens variant)
// Tags: chapter-07, geometry, bitmask, backtracking
//
// MODEL: N-Queens with forbidden squares. One sprinkler per row is
//   forced (n sprinklers, n rows, no two in a row), so the search
//   places row by row and only columns and diagonals carry state.
// MATH: at row r, a column c is usable iff bit c is free in three
//   masks: cols (columns used), diagL (diagonals r - c, shifted left
//   each row), diagR (diagonals r + c, shifted right each row), and
//   the plot (r, c) is not a flowerbed. The candidate set is one AND
//   of complements; extracting bits low-to-high with x & -x walks the
//   candidates without scanning empty positions.
// PROOF: by induction on rows - the three masks summarize exactly which
//   columns/diagonals are attacked in all deeper rows (shifting a
//   diagonal mask by one column per row is the geometry of the
//   diagonal), so a placement is accepted at the leaf iff it is a valid
//   full arrangement, and each arrangement is counted exactly once
//   (each row choice is distinct).
// COMPLEXITY: the search tree is bounded by the N-Queens tree, far
//   below n^n; for n = 13 the full count (no flowerbeds) is 73,712 and
//   the tree is explored in milliseconds. Memory is O(n) stack.
// TYPES: masks fit in unsigned int (n <= 13 bits, and shifted diagonal
//   masks stay under 32 bits if ANDed with the n-bit window each row,
//   which the code does). The count fits comfortably in long long
//   (73,712 at n = 13).
// ALTERNATIVES: permutation enumeration of columns with an O(n) check
//   per permutation is correct but 13! = 6.2e9 - dead on arrival.
//   DP over column bitmasks ignores diagonals; diagonals are the whole
//   problem here, which is why the chapter teaches the three-mask trick.
// EDGE CASES: n = 1 with a free plot (1) and with a flowerbed (0); a
//   fully forbidden row (answer 0, found immediately); no flowerbeds
//   (classic N-Queens counts: 4 -> 2, 8 -> 92); flowerbeds that kill
//   all solutions.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o garden t07-s4-1-sprinkler-garden.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fegarden.exe t07-s4-1-sprinkler-garden.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace {

long long count_placements(int row, unsigned cols, unsigned diag_l, unsigned diag_r,
                           int n, unsigned full, const std::vector<unsigned>& allowed) {
    if (row == n) return 1;
    long long total = 0;
    unsigned candidates = allowed[static_cast<std::size_t>(row)] &
                          full & ~(cols | diag_l | diag_r);
    while (candidates != 0) {
        const unsigned bit = candidates & (~candidates + 1u);  // lowest set bit
        candidates ^= bit;
        total += count_placements(row + 1, cols | bit,
                                  ((diag_l | bit) << 1) & full,
                                  (diag_r | bit) >> 1, n, full, allowed);
    }
    return total;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<unsigned> allowed(static_cast<std::size_t>(n), 0);
    for (int r = 0; r < n; ++r) {
        std::string line;
        std::cin >> line;
        for (int c = 0; c < n; ++c) {
            if (line[static_cast<std::size_t>(c)] == '.') {
                allowed[static_cast<std::size_t>(r)] |= 1u << c;
            }
        }
    }

    const unsigned full = (n == 32) ? ~0u : (1u << n) - 1u;
    std::print("{}\n", count_placements(0, 0, 0, 0, n, full, allowed));
    return 0;
}
