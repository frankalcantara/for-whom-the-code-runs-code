// Exercise A5.5 - The Amphitheater Rows
// Session: A5, exercise 5 of 7 (6 hours for the evaluation)
// Theme: chapter 01 revision (integer widths, overflow discipline,
//   floating-point estimates corrected by exact integer arithmetic)
// Tags: chapter-01
//
// MODEL: the largest k with T(k) = k(k+1)/2 <= B, then the leftover
//   B - T(k). One formula, one inequality - and three different ways
//   to lose points on it at hour five of an evaluation.
// MATH: solving k(k+1)/2 = B exactly gives k = (sqrt(8B+1) - 1) / 2.
//   A long double estimate of that root is computed first, then
//   corrected by at most a few exact integer steps: while
//   T(k+1) <= B, ++k; while T(k) > B, --k. The estimate only needs
//   to land within a few units of the truth, which 64-bit-mantissa
//   long double guarantees for B <= 1e18 (relative error ~1e-19 on
//   x86's 80-bit format; even plain double's ~1e-16 lands within one
//   unit here, the fixup loop forgives both).
// PROOF: T is strictly increasing on k >= 0, so "largest k with
//   T(k) <= B" is well defined and the two correction loops converge
//   to it from either side; termination is immediate because each
//   loop moves k monotonically toward a bound it cannot cross.
// COMPLEXITY: O(1) - one square root and a constant number of
//   correction steps. The trap graded by this exercise is the loop
//   "for k = 1, 2, 3, ... subtract": at B = 1e18, k reaches ~1.4e9
//   iterations - survivable in optimized C++, fatal in the habit it
//   builds.
// TYPES: the overflow ledger, in full (this is the point of the
//   exercise). B <= 1e18 fits int64 (max ~9.22e18). The naive
//   discriminant 8B + 1 reaches 8e18: STILL fits, barely, but the
//   code avoids depending on the margin by using the long double
//   sqrt overload on 2B instead, which is exact enough for the fixup.
//   k <= ~1.41e9, so k * (k + 1) <= ~2e18: fits int64, and T(k) is
//   computed as k * (k + 1) / 2 with the multiplication first -
//   dividing first would truncate odd k. unsigned types are not used:
//   chapter 01's rule that unsigned arithmetic hides exactly the
//   wraparound this exercise exists to expose.
// ALTERNATIVES: binary search on k (chapter 04) is correct and only
//   slightly slower to write - the honest fallback if floating-point
//   error analysis feels risky under a clock; __int128 would absorb
//   every margin above but is nonstandard; the closed-form floor
//   without fixup fails on perfect triangular numbers where the root
//   lands on an integer and the estimate sits a hair below it.
// EDGE CASES: B = 1 (one row, zero left); B = 2 (one row, one left);
//   B triangular (zero leftover, the boundary the fixup exists for);
//   B = 1e18 (the width ceiling); B one below and one above a large
//   triangular number.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o rows a5-05-amphitheater-rows.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Ferows.exe a5-05-amphitheater-rows.cpp

#include <cmath>
#include <cstdint>
#include <iostream>
#include <print>

namespace {

constexpr std::int64_t triangular(std::int64_t k) {
    return k * (k + 1) / 2;  // k <= 1.5e9 keeps the product in range
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::int64_t blocks = 0;
    std::cin >> blocks;

    // Estimate the root of k(k+1)/2 = B as sqrt(2B), then correct
    // with exact integer arithmetic. The estimate is within a couple
    // of units; the loops do the law.
    auto k = static_cast<std::int64_t>(std::sqrt(2.0L * static_cast<long double>(blocks)));
    while (triangular(k + 1) <= blocks) {
        ++k;
    }
    while (triangular(k) > blocks) {
        --k;
    }

    std::println("{} {}", k, blocks - triangular(k));
    return 0;
}
