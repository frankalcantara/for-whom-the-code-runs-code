// Exercise T15.1 - The Marzipan Tower
// Session: T15.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 15 (digit arithmetic and boundary-aware loops)
// Tags: chapter-15
//
// MODEL: count the trailing decimal zeros of n!, without computing
//   n! itself.
// MATH: each trailing zero is one factor 10 = 2 * 5. Factor 2 is
//   more frequent than factor 5 in n!, so the answer is the number
//   of factors of 5:
//   floor(n/5) + floor(n/25) + floor(n/125) + ...
//   Multiples of 25 contribute a second 5, multiples of 125 a
//   third, and so on.
// PROOF: factorization argument. Pair every counted factor 5 with a
//   distinct factor 2 from an even number in the factorial product;
//   there are always enough 2s. Every factor 10 in the product
//   consumes exactly one such pair, so the number of pairs, and
//   therefore trailing zeros, is exactly the number of factors of 5.
// COMPLEXITY: O(log_5 n) time and O(1) memory. At n <= 1e18, this
//   is fewer than 30 loop iterations.
// TYPES: n and the answer fit in signed 64-bit. The loop avoids
//   p *= 5 after p > n / 5, so the multiplier cannot overflow while
//   still being needed.
// ALTERNATIVES: computing the factorial is impossible at this
//   scale; counting factors of 10 directly misses the extra 5s in
//   25, 125, and higher powers.
// EDGE CASES: n < 5 (0); n exactly a power of 5; n near 1e18, where
//   careless p *= 5 loops can overflow; n = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o tower t15-s1-1-marzipan-tower.cpp

#include <cstdint>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::int64_t n = 0;
    std::cin >> n;

    std::int64_t zeros = 0;
    for (std::int64_t power = 5; power <= n;) {
        zeros += n / power;
        if (power > n / 5) break;
        power *= 5;
    }

    std::print("{}\n", zeros);
    return 0;
}
