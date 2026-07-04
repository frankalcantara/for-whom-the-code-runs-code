// Exercise T13.1 - The Tithe Field
// Session: T13.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 13 (shoelace formula, integer-exact area)
// Tags: chapter-13, modular-arithmetic, geometry
//
// MODEL: area of a simple polygon given by its vertices in order.
// MATH: the shoelace formula -
//   2A = | sum_i (x_i * y_{i+1} - x_{i+1} * y_i) |, indices mod n.
//   Each term is the doubled signed area of the triangle
//   (origin, P_i, P_{i+1}); the cross products of consecutive
//   spokes sweep the polygon once, and everything outside cancels
//   in pairs. The sign of the sum is the orientation
//   (counterclockwise positive); the absolute value forgives a
//   sexton who walked the other way.
// PROOF: induction on n by ear clipping - a simple polygon has an
//   ear (two boundary-adjacent vertices whose connecting diagonal
//   lies inside); removing it subtracts exactly the ear triangle's
//   signed area from the sum, and the triangle base case is the
//   cross-product definition. The integer sum is exact: no
//   floating point enters until the final print of X/2.
// COMPLEXITY: O(n) time, O(1) memory - the loop needs only the
//   previous vertex and the first one.
// TYPES: |x|, |y| <= 1e6 so each cross term is <= 2e12 in
//   magnitude, and n = 1e5 terms bound the sum by 2e17: long long
//   holds it with room. The halving is done in integers: 2A is
//   computed, printed as 2A/2 and ".0" or ".5" by parity - no
//   double, no rounding policy, no regrets.
// ALTERNATIVES: triangulating explicitly - what the formula
//   already is, with more code; Pick's theorem - counts lattice
//   points, answers a different question; computing in double -
//   loses integers above 2^53 in general and dignity always.
// EDGE CASES: clockwise input (absolute value); collinear
//   consecutive stones (zero-area ears: harmless); a long thin
//   sliver (cancellation is exact in integers); n = 3; the
//   half-unit area (the example - parity prints .5).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tithe t13-s1-1-tithe-field.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetithe.exe t13-s1-1-tithe-field.cpp

#include <cstdlib>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;

    long long first_x = 0;
    long long first_y = 0;
    long long prev_x = 0;
    long long prev_y = 0;
    long long doubled = 0;
    for (std::size_t i = 0; i < n; ++i) {
        long long x = 0;
        long long y = 0;
        std::cin >> x >> y;
        if (i == 0) {
            first_x = x;
            first_y = y;
        } else {
            doubled += prev_x * y - x * prev_y;
        }
        prev_x = x;
        prev_y = y;
    }
    doubled += prev_x * first_y - first_x * prev_y;  // closing edge
    doubled = std::llabs(doubled);

    std::print("{}.{}\n", doubled / 2, (doubled % 2 == 0) ? '0' : '5');
    return 0;
}
