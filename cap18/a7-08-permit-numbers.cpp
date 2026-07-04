// Exercise A7.8 - The Permit Numbers
// Session: A7, exercise 8 of 9
// Theme: chapters 10 and 11 (modular binomial coefficients)
// Tags: chapter-10, chapter-11, number-theory, modular-arithmetic
//
// MODEL: answer many n choose k queries modulo a prime.
// MATH: precompute factorials and inverse factorials modulo p.
//   Since p = 1e9+7 is prime and n < p, n! has a multiplicative
//   inverse. Then C(n,k) = fact[n] * inv_fact[k] * inv_fact[n-k].
//   Fermat's theorem gives x^(p-2) as the inverse of x modulo p.
// PROOF: factorials count ordered arrangements; dividing by k! and
//   (n-k)! removes the internal orders of chosen and unchosen
//   positions. Modular inverses perform this division in the finite
//   field modulo p. The precomputed arrays store exactly those
//   factors for every queried n.
// COMPLEXITY: O(max_n + q log p) if the inverse of max_n! is found
//   by exponentiation and the remaining inverse factorials are
//   filled downward; each query is O(1). Memory is O(max_n).
// TYPES: products of two residues need 64-bit before the modulo
//   reduction. Indices fit int.
// ALTERNATIVES: Pascal DP is O(max_n^2), far too large; computing
//   each query independently repeats factorial work q times.
// EDGE CASES: k > n (0); k = 0 or k = n (1); n = 0; many repeated
//   queries; maximum n.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o permits a7-08-permit-numbers.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fepermits.exe a7-08-permit-numbers.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace {

constexpr long long kMod = 1'000'000'007LL;

long long mod_pow(long long base, long long exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1LL) result = result * base % kMod;
        base = base * base % kMod;
        exp >>= 1LL;
    }
    return result;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q = 0;
    std::cin >> q;
    std::vector<std::pair<int, int>> queries(q);
    int max_n = 0;
    for (auto& [n, k] : queries) {
        std::cin >> n >> k;
        max_n = std::max(max_n, n);
    }

    std::vector<long long> fact(max_n + 1, 1);
    for (int i = 1; i <= max_n; ++i) fact[i] = fact[i - 1] * i % kMod;

    std::vector<long long> inv_fact(max_n + 1, 1);
    inv_fact[max_n] = mod_pow(fact[max_n], kMod - 2);
    for (int i = max_n; i >= 1; --i) {
        inv_fact[i - 1] = inv_fact[i] * i % kMod;
    }

    for (const auto [n, k] : queries) {
        if (k < 0 || k > n) {
            std::print("0\n");
        } else {
            const long long answer =
                fact[n] * inv_fact[k] % kMod * inv_fact[n - k] % kMod;
            std::print("{}\n", answer);
        }
    }
    return 0;
}
