// Exercise T03.7 - The Glacier Gauge
// Session: T03.S4, exercise 3 of 3 (2 hours for the three; this one resists)
// Theme: chapters 01 + 03 (truncated vs floored division; exact integer
//   reporting at streaming scale)
// Tags: chapter-01, chapter-03
//
// MODEL: given the total T of n signed readings, publish g and r with
//   T = g * n + r and 0 <= r < n. This pair is unique: it is Euclidean
//   division of T by n.
// MATH: C++ integer division truncates toward zero, so for negative T it
//   rounds the wrong way for this contract: -3 / 4 == 0 with remainder -3,
//   and (0, -3) violates 0 <= r < n. Floored division rounds toward minus
//   infinity. The repair is one conditional step:
//     g = T / n;  r = T % n;  if (r < 0) { r += n; g -= 1; }
//   which preserves g * n + r = T because the two changes cancel:
//     (g - 1) * n + (r + n) = g * n + r.
// PROOF: existence and uniqueness of Euclidean division: among all pairs
//   (g, r) with T = g*n + r, exactly one has r in [0, n). The truncated
//   pair differs from it by at most this one correction, and the algebra
//   above shows the corrected pair still reconstructs T. The plaque about
//   floating point exists because double holds 53 bits of mantissa and
//   T can need up to 54: 1e7 * 1e9 = 1e16 > 2^53 ~ 9.0e15. The bulletin
//   would publish a rounded total and call it exact.
// COMPLEXITY: O(n) time, O(1) memory; the readings are never stored.
// TYPES: |T| <= 1e16: long long. n fits in int; the correction arithmetic
//   stays in long long throughout, because g * n is reconstructed by
//   offices with 64-bit calculators.
// ALTERNATIVES: std::div mirrors the truncated pair and needs the same
//   correction; computing with double is the plaque. Unsynced std::cin is
//   used here instead of the fread parser to show the lighter Chapter 3
//   layer carrying 1e7 values acceptably; the parser remains the tool when
//   the limit tightens.
// EDGE CASES: T negative and not divisible by n (the correction fires);
//   T negative and divisible (it must not fire); T = 0; n = 1 (r is
//   always 0); all readings equal.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o glacier t03-s4-3-glacier-gauge.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feglacier.exe t03-s4-3-glacier-gauge.cpp

#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n = 0;
    std::cin >> n;

    long long total = 0;
    for (long long i = 0; i < n; ++i) {
        long long reading = 0;
        std::cin >> reading;
        total += reading;
    }

    long long g = total / n;
    long long r = total % n;
    if (r < 0) { r += n; g -= 1; }

    std::print("{} {}\n", g, r);
    return 0;
}
