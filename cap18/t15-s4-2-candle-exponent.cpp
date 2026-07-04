// Exercise T15.6 - The Candle Exponent
// Session: T15.S4, exercise 2 of 3 (2 hours for all three)
// Theme: chapter 15 (big integer by decimal digits)
// Tags: chapter-15, modular-arithmetic
//
// MODEL: compute the decimal digit sum of a^b for small base a and
//   large exponent b.
// MATH: store decimal digits in little-endian order. Multiplying by
//   a applies grade-school multiplication: new_digit =
//   (old_digit * a + carry) mod 10, and carry continues to the next
//   digit. Repeating this b times constructs a^b exactly.
// PROOF: loop invariant. After t multiplications, the digit vector
//   represents a^t in base 10. The multiplication pass is exactly
//   the base-10 positional identity, so after the next pass it
//   represents a^(t+1). Summing the final digits gives the requested
//   value.
// COMPLEXITY: O(b * D) time, where D is the number of decimal
//   digits of a^b, and O(D) memory. At the limits D is under 10000.
// TYPES: each digit and carry are small int values. The digit sum is
//   at most 9D, also an int.
// ALTERNATIVES: arbitrary-precision libraries are convenient but
//   hide the chapter's intended representation exercise; built-in
//   integers overflow almost immediately.
// EDGE CASES: b = 0 (a^0 = 1); a = 1; carry producing multiple new
//   digits; large b.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o candle t15-s4-2-candle-exponent.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecandle.exe t15-s4-2-candle-exponent.cpp

#include <iostream>
#include <numeric>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int base = 0;
    int exponent = 0;
    std::cin >> base >> exponent;

    std::vector<int> digits{1};
    for (int step = 0; step < exponent; ++step) {
        int carry = 0;
        for (int& digit : digits) {
            const int value = digit * base + carry;
            digit = value % 10;
            carry = value / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    const int sum = std::accumulate(digits.begin(), digits.end(), 0);
    std::print("{}\n", sum);
    return 0;
}
