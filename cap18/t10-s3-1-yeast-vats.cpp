// Exercise T10.3 - The Yeast Vats
// Session: T10.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 10 (modular exponentiation by squaring)
// Tags: chapter-10, number-theory, modular-arithmetic, fast-exponentiation
//
// MODEL: compute a^b mod p with b up to 1e18. Multiplying b times is
//   1e18 operations; the binary expansion of b cuts it to ~60.
// MATH: a^b = product over set bits i of b of a^(2^i), and each
//   a^(2^i) is the square of the previous one. The loop keeps
//   (base, result): square base each step, multiply it into result
//   when the current bit of b is 1.
// PROOF: induction on bits processed - after k steps, result holds
//   a^(b mod 2^k) and base holds a^(2^k), both mod p; the update
//   preserves it for k+1 by the exponent law a^(x+y) = a^x a^y.
//   Termination after ~log2(b) <= 60 steps.
// COMPLEXITY: O(log b) multiplications, O(1) memory. This routine is
//   infrastructure: Fermat inverses (T10.7), CRT recombination,
//   hashing, primality tests all sit on top of it; it must come out
//   of the fingers without thought.
// TYPES: the working trap of the exercise. p ~ 1e9, so residues fit
//   in long long, but the PRODUCT of two residues reaches ~1e18 -
//   inside long long (max 9.2e18), which is exactly why the code
//   reduces a mod p BEFORE the loop: a itself can be 1e18, and
//   1e18 * 1e18 overflows everything short of 128 bits. With both
//   factors reduced, x * y <= (1e9+6)^2 ~ 1e18: safe. For moduli up
//   to 9e18 the same code needs unsigned __int128 in the multiply -
//   one comment so the future reader of this header knows where the
//   cliff is.
// ALTERNATIVES: recursion (halve the exponent) is the same idea with
//   call overhead; addition chains beat binary by a few
//   multiplications and are not worth finding; for FIXED base and
//   many exponents, precomputed power tables (baby-step style) win -
//   not the shape here.
// EDGE CASES: b = 0 (empty product: 1, including 0^0 = 1 by the
//   combinatorial convention the ledger adopts); a = 0 with b > 0
//   (0); a multiple of p (0 for b > 0 - the reduction handles it);
//   a = 1e18, b = 1e18 (the overflow drill end to end).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o yeast t10-s3-1-yeast-vats.cpp

#include <iostream>
#include <print>

namespace {

long long power_mod(long long base, long long exponent, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    if (base < 0) base += mod;
    while (exponent > 0) {
        if ((exponent & 1) == 1) {
            result = result * base % mod;  // both factors < mod ~ 1e9: fits
        }
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
    std::cin >> a >> b;

    constexpr long long kMod = 1'000'000'007;
    std::print("{}\n", power_mod(a, b, kMod));
    return 0;
}
