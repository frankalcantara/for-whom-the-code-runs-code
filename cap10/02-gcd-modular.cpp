// cap10/02-gcd-modular.cpp
// GCD, LCM, modular arithmetic, fast exponentiation, modular inverse, CRT.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o gcd_mod 02-gcd-modular.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fegcd_mod.exe 02-gcd-modular.cpp

#include <cassert>
#include <print>
#include <utility>

using i64 = long long;

// ============================================================
// GCD and LCM: O(log min(a,b)).
// std::gcd has existed since C++17, but the manual version keeps
// the Euclidean invariant visible.
// ============================================================
i64 gcd(i64 a, i64 b) { return b == 0 ? a : gcd(b, a % b); }
i64 lcm(i64 a, i64 b) { return a / gcd(a, b) * b; }  // divide first to reduce overflow risk

// ============================================================
// Extended Euclidean algorithm: O(log min(a,b)).
// Finds (x, y) such that a*x + b*y = gcd(a,b).
// Used for modular inverses when the modulus is not necessarily prime.
// ============================================================
i64 ext_gcd(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    i64 x1, y1;
    i64 d = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return d;
}

// ============================================================
// Fast modular exponentiation: O(log exp).
// Computes base^exp % mod while reducing after each multiplication.
// ============================================================
i64 pow_mod(i64 base, i64 exp, i64 mod) {
    i64 result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// ============================================================
// Modular inverse: O(log mod).
// inv_mod(a, mod) returns x such that a*x == 1 (mod mod).
// Requirement: gcd(a, mod) == 1.
// Fermat works for prime moduli; extended Euclid works for any coprime pair.
// ============================================================
i64 inv_mod_fermat(i64 a, i64 mod) {
    // Requires a prime modulus.
    return pow_mod(a, mod - 2, mod);
}

i64 inv_mod(i64 a, i64 mod) {
    i64 x, y;
    i64 d = ext_gcd(a, mod, x, y);
    (void)d;  // d must be 1 for the inverse to exist.
    return (x % mod + mod) % mod;
}

// ============================================================
// Chinese Remainder Theorem (CRT): O(log min(m1,m2)).
// Given x == r1 (mod m1), x == r2 (mod m2), with gcd(m1,m2)=1,
// returns (r, m) such that x == r (mod m), m = m1*m2.
// ============================================================
std::pair<i64,i64> crt(i64 r1, i64 m1, i64 r2, i64 m2) {
    i64 x, y;
    i64 d = ext_gcd(m1, m2, x, y);
    (void)d;  // must be 1 for coprime moduli
    i64 m = m1 * m2;
    // Combine: r = r1 + m1 * t, where m1*t == r2-r1 (mod m2).
    i64 r = (r1 + m1 % m * ((r2 - r1) % m2 * x % m2) % m) % m;
    if (r < 0) r += m;
    return {r, m};
}

int main() {
    // gcd / lcm
    assert(gcd(48, 18) == 6);
    assert(gcd(100, 75) == 25);
    assert(lcm(4, 6) == 12);
    assert(lcm(12, 18) == 36);

    // ext_gcd: 3*x + 5*y = 1
    {
        i64 x, y;
        i64 d = ext_gcd(3, 5, x, y);
        assert(d == 1);
        assert(3*x + 5*y == 1);
    }

    // pow_mod
    const i64 MOD = 1000000007;
    assert(pow_mod(2, 10, MOD) == 1024);
    assert(pow_mod(2, 30, MOD) == 1073741824 % MOD);
    assert(pow_mod(3, MOD - 1, MOD) == 1);   // Fermat's Little Theorem

    // inv_mod: 3 * inv(3) == 1 (mod 7)
    assert(inv_mod(3, 7) == 5);               // 3*5=15 == 1 (mod 7)
    assert(inv_mod_fermat(3, MOD) * 3 % MOD == 1);

    // crt: x == 2 (mod 3), x == 3 (mod 5) -> x == 8 (mod 15)
    auto [r, m] = crt(2, 3, 3, 5);
    assert(m == 15);
    assert(r % 3 == 2 && r % 5 == 3);

    std::print("02-gcd-modular: all assertions passed\n");
    return 0;
}
