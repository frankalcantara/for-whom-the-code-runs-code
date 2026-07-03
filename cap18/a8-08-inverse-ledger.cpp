// Exercise A8.8 - The Inverse Ledger
// Session: A8, exercise 8 of 10
// Theme: chapter 10 (modular inverse by exponentiation)
// Tags: chapter-10, number-theory, modular-arithmetic
//
// MODEL: compute a / b modulo prime p = 1e9+7.
// MATH: since b is not divisible by p, Fermat's theorem gives
//   b^(p-2) as b^{-1} modulo p.
// PROOF: in the nonzero residue field modulo p, b^(p-1)=1, so
//   multiplying both sides by b^{-1} yields b^(p-2)=b^{-1}.
// COMPLEXITY: O(log p) time and O(1) memory.
// TYPES: products of residues need int64_t before taking modulo.
// ALTERNATIVES: extended Euclid also works; exponentiation is direct
//   for a prime modulus.
// EDGE CASES: a=0; b larger than p; b mod p = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o inverse a8-08-inverse-ledger.cpp

#include <cstdint>
#include <iostream>
#include <print>

namespace {
constexpr std::int64_t kMod = 1'000'000'007LL;

std::int64_t mod_pow(std::int64_t base, std::int64_t exp) {
    std::int64_t result = 1;
    base %= kMod;
    while (exp > 0) {
        if (exp & 1LL) result = result * base % kMod;
        base = base * base % kMod;
        exp >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    std::int64_t a = 0, b = 0;
    std::cin >> a >> b;
    std::print("{}\n", (a % kMod) * mod_pow(b, kMod - 2) % kMod);
    return 0;
}
