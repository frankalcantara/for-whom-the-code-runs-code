// Exercise A6.1 - The Festival Favors
// Session: A6, exercise 1 of 8 (6 hours for the set)
// Theme: chapter 11 (surjections by inclusion-exclusion; chapter 10
//   supplies modular power and inverse)
// Tags: chapter-11, chapter-10, number-theory, modular-arithmetic
//
// MODEL: functions from n distinct favors ONTO k distinct guests -
//   surjections, counted modulo p.
// MATH: count all functions, k^n, and subtract those missing at
//   least one guest. Let A_i = functions avoiding guest i; any
//   fixed j-subset of guests excluded leaves (k-j)^n functions.
//   Inclusion-exclusion:
//   surj(n, k) = sum_{j=0}^{k} (-1)^j C(k, j) (k - j)^n.
// PROOF: a function missing exactly e >= 1 guests is counted by
//   the alternating sum with weight sum_{j<=e} (-1)^j C(e, j) = 0,
//   and a surjection (e = 0) with weight 1 - the binomial theorem
//   on (1-1)^e. So only surjections survive.
// COMPLEXITY: O(k) factorial table for C(k, j), O(k log n) for the
//   k+1 modular powers. At k = 1e6, ~3e7 multiplications: fine.
// TYPES: residue products ~1e18 in long long; exponent n <= 1e6
//   fits everywhere; the alternating sum is kept non-negative by
//   adding kMod - term instead of subtracting.
// ALTERNATIVES: k! * S(n, k) via the Stirling DP is O(nk) - 1e12
//   at the limits, dead; the IE sum IS the practical closed form.
//   Exponential generating functions say (e^x - 1)^k and reduce to
//   the same sum.
// EDGE CASES: k = 1 (1 - everything to the only guest); k = n
//   (n! - a bijection); k = 2 (2^n - 2, the example's family);
//   n = k = 1; n = 1e6 with k = 1 (power loop dominates).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o favors a6-01-festival-favors.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fefavors.exe a6-01-festival-favors.cpp

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

    const std::size_t size = static_cast<std::size_t>(k) + 1;
    std::vector<long long> fact(size, 1);
    for (std::size_t i = 1; i < size; ++i) {
        fact[i] = fact[i - 1] * static_cast<long long>(i) % kMod;
    }
    std::vector<long long> inv_fact(size, 1);
    inv_fact[size - 1] = power_mod(fact[size - 1], kMod - 2);
    for (std::size_t i = size - 1; i > 0; --i) {
        inv_fact[i - 1] = inv_fact[i] * static_cast<long long>(i) % kMod;
    }

    long long sum = 0;
    for (long long j = 0; j <= k; ++j) {
        const auto uj = static_cast<std::size_t>(j);
        const auto uk = static_cast<std::size_t>(k);
        const long long binom =
            fact[uk] * inv_fact[uj] % kMod * inv_fact[uk - uj] % kMod;
        const long long term = binom * power_mod(k - j, n) % kMod;
        sum += (j % 2 == 0) ? term : kMod - term;
        sum %= kMod;
    }
    std::print("{}\n", sum);
    return 0;
}
