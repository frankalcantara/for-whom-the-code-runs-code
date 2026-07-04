// Exercise A6.5 - The Astronomer's Cycle
// Session: A6, exercise 5 of 8 (6 hours for the set)
// Theme: chapter 10 revision (Fermat's little theorem applied to
//   the exponent; modular exponentiation at two moduli)
// Tags: chapter-10, number-theory, modular-arithmetic, fast-exponentiation
//
// MODEL: a^(b^c) mod p with all three inputs up to 1e18 - the
//   exponent b^c does not fit any machine word, so it must be
//   reduced BEFORE it exists.
// MATH: p = 1e9+7 is prime. For a not divisible by p, Fermat gives
//   a^(p-1) = 1 (mod p), so exponents matter only mod p-1:
//   a^(b^c) = a^(b^c mod (p-1)) (mod p). Compute
//   e = power_mod(b mod (p-1), c, p-1), then power_mod(a, e, p).
//   For a divisible by p the answer is 0 (the exponent b^c >= 1
//   since b, c >= 1) - Fermat does not apply and is not needed.
// PROOF: Fermat's little theorem (group of units of Z_p has order
//   p-1, Lagrange); write b^c = q(p-1) + e, then
//   a^(b^c) = (a^(p-1))^q * a^e = a^e. Reducing the BASE b mod
//   (p-1) before exponentiating is sound because the reduction is
//   a ring homomorphism Z -> Z_{p-1}: b^c mod (p-1) depends on b
//   only through b mod (p-1). The divisible case: a = pt gives
//   a^(b^c) = p^(b^c) t^(b^c) = 0 mod p for any positive exponent.
// COMPLEXITY: two modular exponentiations, O(log c + log e) ~ 120
//   multiplications. O(1) memory.
// TYPES: the working trap, twice over. Residues mod p-1 reach
//   ~1e9 and their products ~1e18: long long holds them (the same
//   arithmetic as mod p). The EXPONENT c is used un-reduced - it
//   is a count of squarings, not a residue - and fits long long by
//   the constraint. Reducing c mod (p-1) would be the classic
//   wrong answer: the theorem reduces the exponent OF a, not the
//   exponent OF b.
// ALTERNATIVES: Euler's theorem generalizes to composite moduli
//   with phi, plus the "exponent >= log2(mod)" guard for
//   non-coprime bases (the lifting-the-exponent-style caveat);
//   here the modulus is prime and the coprime/divisible split is
//   the whole story. __int128 buys nothing - b^c overflows it for
//   c >= 2 already at b = 2e19^(1/2).
// EDGE CASES: a multiple of p (0 - e.g. a = p itself); b multiple
//   of p-1 (exponent reduces to 0: answer 1); b = 1 or c arbitrary
//   with b^c mod (p-1) = 1 (answer a mod p); a = b = c = 1 (1);
//   all three at 1e18.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o cycle a6-05-astronomers-cycle.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecycle.exe a6-05-astronomers-cycle.cpp

#include <iostream>
#include <print>

namespace {

long long power_mod(long long base, long long exponent, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exponent > 0) {
        if ((exponent & 1) == 1) result = result * base % mod;
        base = base * base % mod;
        exponent >>= 1;
    }
    return result;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long a = 0;
    long long b = 0;
    long long c = 0;
    std::cin >> a >> b >> c;

    constexpr long long kMod = 1'000'000'007;
    if (a % kMod == 0) {
        std::print("0\n");  // b^c >= 1, so the product collapses
        return 0;
    }
    const long long reduced_exponent = power_mod(b, c, kMod - 1);
    std::print("{}\n", power_mod(a, reduced_exponent, kMod));
    return 0;
}
