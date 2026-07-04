// Exercise T01.2 - The Kilnmaster's Bill
// Session: T01.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 01 (closed formula instead of a loop, overflow arithmetic)
// Tags: chapter-01
//
// MODEL: compute sum_{k=a}^{b} k for 1 <= a <= b <= 2e9.
// MATH: pair the terms from both ends: (a + b), (a+1 + b-1), and so on.
//   Every pair sums a + b and there are b - a + 1 terms, so
//     total = (a + b)(b - a + 1) / 2.
//   A loop from a to b can visit 2e9 values; the formula visits none.
// PROOF: the pairing is a bijection between terms and half-pairs when the
//   count is even; when the count is odd the middle term equals (a+b)/2 and
//   the same product formula holds. Alternatively, induction on b.
// COMPLEXITY: O(1) time and memory.
// TYPES: a + b <= 4e9 and b - a + 1 <= 2e9 both overflow int. Exactly one
//   of the two factors is even; dividing that factor by 2 *before* the
//   multiplication bounds the product by ~2.1e18 < 2^63 - 1. Multiplying
//   first would still fit (~4e18) but with the kind of margin that vanishes
//   the day a constraint grows. The maximum answer, a = 1 and b = 2e9, is
//   2000000001000000000.
// ALTERNATIVES: a loop is correct and roughly 2e9 iterations too slow to be
//   respectable; __int128 widens the margin but is unnecessary here.
// EDGE CASES: a == b (single batch); a = 1, b = 2e9 (maximum total); pairs
//   where a + b is odd (the even factor is then b - a + 1).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o kiln t01-s2-1-kilnmaster-bill.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fekiln.exe t01-s2-1-kilnmaster-bill.cpp

#include <iostream>
#include <print>

int main() {
    long long a = 0;
    long long b = 0;
    std::cin >> a >> b;

    long long first_last = a + b;       // <= 4e9, needs 64 bits already
    long long count = b - a + 1;        // <= 2e9, same
    long long total = (first_last % 2 == 0)
                          ? (first_last / 2) * count
                          : first_last * (count / 2);

    std::print("{}\n", total);
    return 0;
}
