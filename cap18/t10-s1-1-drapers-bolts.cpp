// Exercise T10.1 - The Drapers' Bolts
// Session: T10.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 10 (gcd by the Euclidean algorithm; lcm avoided on
//   purpose), with chapter 01's width discipline
// Tags: chapter-10, chapter-01, number-theory, modular-arithmetic, gcd
//
// MODEL: the legal piece lengths are exactly the common divisors of a
//   and b; the longest is gcd(a, b). The piece count is then
//   a/g + b/g - and the warm-up's only teeth are in the types.
// MATH: gcd(a, b) = gcd(b, a mod b), gcd(x, 0) = x. Each two steps at
//   least halve the larger argument, so the algorithm runs in
//   O(log min(a, b)) divisions - about 60 worst case at 1e18 (the
//   worst inputs are consecutive Fibonacci numbers, Lame's theorem).
// PROOF: d divides both a and b iff d divides both b and a mod b
//   (a mod b = a - qb is an integer combination, and back); so the
//   common-divisor SET is invariant under the step, hence so is its
//   maximum; the base case gcd(x, 0) = x is the definition. That the
//   answer must divide both bolts exactly is the no-scrap law; that
//   every common divisor works is the construction; so the maximum
//   common divisor is exactly the legal optimum.
// COMPLEXITY: O(log min(a, b)) time, O(1) memory. Nothing else in the
//   training program costs this little and is this easy to get wrong
//   in width.
// TYPES: a, b reach 1e18: long long (they fit, 2^63-1 ~ 9.2e18). The
//   piece count a/g + b/g <= a + b <= 2e18, which fits - but barely,
//   and ONLY because the divisions happen before the addition.
//   Computing lcm(a, b) = a/g*b first would overflow at these limits
//   even in unsigned long long; the exercise's statement is shaped to
//   make the safe expression the natural one, and the header says so
//   for the day a variant asks for the lcm (divide before multiplying,
//   and check against 9.2e18 anyway).
// ALTERNATIVES: std::gcd from <numeric> is the production answer
//   (used here); binary gcd (shifts instead of divisions) wins on
//   machines where division is slow - C++17's std::gcd already
//   delegates well; factorizing both numbers to intersect exponents
//   is O(sqrt a) - a thousand times slower at 1e18 and the wrong
//   instinct to train.
// EDGE CASES: a = b (one piece length, two pieces... a/g + b/g = 2);
//   a divides b; coprime bolts (pieces of length 1, a + b of them -
//   the 2e18 fit); a = b = 1; a = b = 1e18 (count 2, no overflow).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o bolts t10-s1-1-drapers-bolts.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febolts.exe t10-s1-1-drapers-bolts.cpp

#include <iostream>
#include <numeric>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long a = 0;
    long long b = 0;
    std::cin >> a >> b;

    const long long g = std::gcd(a, b);
    const long long pieces = a / g + b / g;  // divide first: a/g*b would overflow
    std::print("{} {}\n", g, pieces);
    return 0;
}
