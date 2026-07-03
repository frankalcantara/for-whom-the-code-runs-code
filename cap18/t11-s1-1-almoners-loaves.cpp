// Exercise T11.1 - The Almoner's Loaves
// Session: T11.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 11 (stars and bars, binomial under a prime modulus)
// Tags: chapter-11, number-theory, modular-arithmetic
//
// MODEL: distributions of n identical items among k labeled boxes,
//   empty boxes allowed - the number of solutions of
//   x1 + x2 + ... + xk = n with xi >= 0 integers.
// MATH: stars and bars. Write n stars in a row and insert k-1 bars;
//   the stars between consecutive bars are one petitioner's loaves.
//   Every arrangement of n stars and k-1 bars in n+k-1 positions is
//   one distribution and vice versa, so the count is
//   C(n+k-1, k-1). Computed with factorials and Fermat inverses
//   modulo p = 1e9+7 (chapter 10 supplies the inverse).
// PROOF: the star/bar encoding is a bijection - given a solution
//   (x1..xk), lay down x1 stars, a bar, x2 stars, ... ; given an
//   arrangement, read the gaps. Both directions are total and
//   inverse to each other, so the sets have equal size.
// COMPLEXITY: O(n+k) to build factorial and inverse-factorial
//   tables, O(1) per query. Memory O(n+k). n+k <= 2e6: trivial.
// TYPES: factorials are residues < 1e9+7, products of two residues
//   reach ~1e18 - long long holds them (max 9.2e18). Table indices
//   reach 2e6 - 1: int would do, size_t is used for indexing.
// ALTERNATIVES: multiplying C(n+k-1, k-1) digit by digit with big
//   integers - the answer has ~600k digits at the limits, hopeless;
//   Pascal's triangle DP is O((n+k)^2) memory - also hopeless. The
//   factorial table is the only sane route at 1e6.
// EDGE CASES: n = 0 (one distribution: everyone goes hungry,
//   C(k-1, k-1) = 1); k = 1 (one distribution, all loaves to the
//   only petitioner); n = k = 1; both at 1e6 (table size 2e6 - 1,
//   no overflow in the table build).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o loaves t11-s1-1-almoners-loaves.cpp

#include <iostream>
#include <print>
#include <vector>

namespace {

constexpr long long kMod = 1'000'000'007;

long long power_mod(long long base, long long exponent) {
    long long result = 1;
    base %= kMod;
    while (exponent > 0) {
        if ((exponent & 1) == 1) result = result * base % kMod;
        base = base * base % kMod;
        exponent >>= 1;
    }
    return result;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n = 0;
    long long k = 0;
    std::cin >> n >> k;

    const std::size_t size = static_cast<std::size_t>(n + k);  // need n+k-1
    std::vector<long long> fact(size, 1);
    for (std::size_t i = 1; i < size; ++i) {
        fact[i] = fact[i - 1] * static_cast<long long>(i) % kMod;
    }
    std::vector<long long> inv_fact(size, 1);
    inv_fact[size - 1] = power_mod(fact[size - 1], kMod - 2);
    for (std::size_t i = size - 1; i > 0; --i) {
        inv_fact[i - 1] = inv_fact[i] * static_cast<long long>(i) % kMod;
    }

    const std::size_t top = static_cast<std::size_t>(n + k - 1);
    const std::size_t choose = static_cast<std::size_t>(k - 1);
    const long long answer =
        fact[top] * inv_fact[choose] % kMod * inv_fact[top - choose] % kMod;
    std::print("{}\n", answer);
    return 0;
}
