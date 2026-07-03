// Exercise T15.7 - The Counting Stamp
// Session: T15.S4, exercise 3 of 3 (2 hours for all three)
// Theme: chapter 15 (factorial zeros) with chapter 04's answer
//   search
// Tags: chapter-15, chapter-04, binary-search-answer, binary-search
//
// MODEL: find the smallest n whose factorial has at least z trailing
//   decimal zeros.
// MATH: zeros(n) = floor(n/5) + floor(n/25) + ... counts factors
//   of 5 in n!. The function zeros(n) is monotone nondecreasing, so
//   binary search can find the first n with zeros(n) >= z.
// PROOF: factorization gives the zero-count formula. Monotonicity
//   follows because adding more factors to the factorial cannot
//   remove factors of 5. Binary search over a monotone predicate
//   returns the first true position. The high bound 5z is enough for
//   z > 0 because floor(5z/5) = z already contributes z factors.
// COMPLEXITY: O(log z * log_5 z) time and O(1) memory.
// TYPES: z <= 1e18, so high = 5z <= 5e18 fits signed int64_t.
//   The zero count also fits int64_t under these bounds. The power
//   loop avoids overflowing its multiplier.
// ALTERNATIVES: linear search is hopeless; inverting the sum with a
//   closed form is unnecessary and error-prone around jumps.
// EDGE CASES: z = 0 (answer 0); z at a jump such as 6 -> 25; large
//   z near 1e18.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o stamp t15-s4-3-counting-stamp.cpp

#include <cstdint>
#include <iostream>
#include <print>

namespace {

std::int64_t trailing_zeros(std::int64_t n) {
    std::int64_t count = 0;
    for (std::int64_t power = 5; power <= n;) {
        count += n / power;
        if (power > n / 5) break;
        power *= 5;
    }
    return count;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::int64_t z = 0;
    std::cin >> z;
    if (z == 0) {
        std::print("0\n");
        return 0;
    }

    std::int64_t low = 0;
    std::int64_t high = 5 * z;
    while (low < high) {
        const std::int64_t mid = low + (high - low) / 2;
        if (trailing_zeros(mid) >= z) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    std::print("{}\n", low);
    return 0;
}
