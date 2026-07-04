// Exercise T01.4 - The Star Atlas
// Session: T01.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 01 (space complexity, memory limits, overflow in products)
// Tags: chapter-01
//
// MODEL: a grid needs bytes(r, c) = 8 * r * c bytes; the budget is
//   B * 2^20 bytes. Decide bytes <= budget and report ceil(bytes / 2^20).
// MATH: the ceiling of an integer division is
//   ceil(p / q) = (p + q - 1) / q
//   with truncating division, valid for p >= 0 and q > 0. Here q = 2^20.
// PROOF: arithmetic only. bytes counts cells times cell width exactly;
//   the comparison and the ceiling are exact in integers, so every verdict
//   follows from the definition of the contract.
// COMPLEXITY: O(1) per proposal, O(q) total.
// TYPES: r * c reaches 1e18 and 8 * r * c reaches 8e18. That fits in
//   unsigned long long (< 1.8e19) with room for the +2^20-1 of the ceiling,
//   and would *barely* fit in long long; the product r * c computed in int
//   is garbage long before that. The multiplication must be performed in a
//   64-bit type from the first factor, not converted after overflowing.
// ALTERNATIVES: comparing r <= budget / (8 * c) avoids the large product
//   entirely; it is the right tool when even the wide type cannot hold the
//   product. Here the product fits, and reporting the exact required size
//   needs it anyway.
// EDGE CASES: 1x1 grid (8 bytes, requires 1 MiB after rounding up); a grid
//   of exactly B mebibytes (fits, no rounding); r = c = 1e9 (the 8e18
//   maximum); budget far larger than any grid.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o atlas t01-s3-2-star-atlas.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Featlas.exe t01-s3-2-star-atlas.cpp

#include <cstdint>
#include <iostream>
#include <print>

int main() {
    constexpr std::uint64_t MIB = 1ULL << 20;

    int q = 0;
    std::cin >> q;

    while (q-- > 0) {
        std::uint64_t r = 0;
        std::uint64_t c = 0;
        std::uint64_t budget_mib = 0;
        std::cin >> r >> c >> budget_mib;

        std::uint64_t bytes = 8ULL * r * c;
        std::uint64_t needed_mib = (bytes + MIB - 1) / MIB;

        if (bytes <= budget_mib * MIB)
            std::print("FITS {}\n", needed_mib);
        else
            std::print("TOO LARGE {}\n", needed_mib);
    }
    return 0;
}
