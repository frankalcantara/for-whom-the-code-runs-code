// Exercise T11.2 - The Portrait Gallery
// Session: T11.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 11 (derangements, binomial coefficients, rencontres)
// Tags: chapter-11, dynamic-programming, modular-arithmetic
//
// MODEL: permutations of n elements with exactly m fixed points -
//   the rencontres numbers D(n, m).
// MATH: pick WHICH m portraits stay put: C(n, m) ways. The other
//   n-m must all move: that is a derangement of n-m elements,
//   counted by D(n-m), with the recurrence
//   D(0) = 1, D(1) = 0, D(t) = (t-1) * (D(t-1) + D(t-2)).
//   Answer: C(n, m) * D(n-m) mod p. Both tables are built once.
// PROOF: the split is a bijection (fixed set, derangement of the
//   rest); the derangement recurrence is the classic case split on
//   where element t goes, say position j != t: either j's element
//   goes to t (swap: D(t-2) for the rest) or it does not (element
//   of j is forbidden exactly one slot: D(t-1)); j has t-1 choices.
//   Inclusion-exclusion gives the same numbers - the recurrence is
//   cheaper and avoids alternating sums.
// COMPLEXITY: O(n) for factorials, inverse factorials and the
//   derangement table; O(1) for the query. Memory O(n).
// TYPES: residues < 1e9+7, pairwise products < ~1e18: long long.
//   n fits int; long long is used end to end to avoid the
//   int-times-int trap inside the table builds.
// ALTERNATIVES: D(t) = round(t!/e) - elegant, useless modulo p;
//   inclusion-exclusion sum per query is O(n) per query, fine here
//   but loses to the recurrence when queries multiply; computing
//   C(n, m) by Pascal: O(n^2) memory, no.
// EDGE CASES: m = n (everything stays: 1); m = n-1 (impossible -
//   one portrait cannot be wrong alone: 0, via D(1) = 0); m = 0
//   (full derangement D(n)); n = 1 with m = 0 (0) and m = 1 (1).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o gallery t11-s2-1-portrait-gallery.cpp

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
    long long m = 0;
    std::cin >> n >> m;

    const std::size_t size = static_cast<std::size_t>(n) + 1;
    std::vector<long long> fact(size, 1);
    for (std::size_t i = 1; i < size; ++i) {
        fact[i] = fact[i - 1] * static_cast<long long>(i) % kMod;
    }
    std::vector<long long> inv_fact(size, 1);
    inv_fact[size - 1] = power_mod(fact[size - 1], kMod - 2);
    for (std::size_t i = size - 1; i > 0; --i) {
        inv_fact[i - 1] = inv_fact[i] * static_cast<long long>(i) % kMod;
    }
    std::vector<long long> derangement(size, 0);
    derangement[0] = 1;
    if (size > 1) derangement[1] = 0;
    for (std::size_t t = 2; t < size; ++t) {
        derangement[t] = static_cast<long long>(t - 1) *
                         ((derangement[t - 1] + derangement[t - 2]) % kMod) %
                         kMod;
    }

    const auto un = static_cast<std::size_t>(n);
    const auto um = static_cast<std::size_t>(m);
    const long long binom =
        fact[un] * inv_fact[um] % kMod * inv_fact[un - um] % kMod;
    std::print("{}\n", binom * derangement[un - um] % kMod);
    return 0;
}
