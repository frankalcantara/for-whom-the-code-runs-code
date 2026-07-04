// cap10/03-combinatorics-mod.cpp
// Factorials, combinations, and binomial coefficients modulo a prime.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o comb_mod 03-combinatorics-mod.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecomb_mod.exe 03-combinatorics-mod.cpp

#include <cassert>
#include <print>
#include <vector>

using i64 = long long;

constexpr i64 MOD = 1000000007;

// ============================================================
// Precomputed factorial table: O(n) preprocessing, O(1) per query.
// C(n,k) = n! / (k! * (n-k)!) mod p, with p prime.
// Division is multiplication by modular inverse.
// ============================================================
struct Combinatorics {
    int n;
    std::vector<i64> fat, inv_fat;

    explicit Combinatorics(int n_) : n(n_), fat(n_+1), inv_fat(n_+1) {
        fat[0] = 1;
        for (int i = 1; i <= n_; ++i)
            fat[i] = fat[i-1] * i % MOD;
        // Invert fat[n] with Fermat, then propagate inverses downward.
        inv_fat[n_] = pow_mod(fat[n_], MOD - 2);
        for (int i = n_ - 1; i >= 0; --i)
            inv_fat[i] = inv_fat[i+1] * (i+1) % MOD;
    }

    // C(a, b) mod p: O(1).
    i64 C(int a, int b) const {
        if (b < 0 || b > a) return 0;
        return fat[a] % MOD * inv_fat[b] % MOD * inv_fat[a-b] % MOD;
    }

private:
    static i64 pow_mod(i64 base, i64 exp) {
        i64 result = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp & 1) result = result * base % MOD;
            base = base * base % MOD;
            exp >>= 1;
        }
        return result;
    }
};

// ============================================================
// Pascal triangle: C(n,k) without division, O(n^2) memory.
// Useful when the modulus is not prime or n is small.
// ============================================================
std::vector<std::vector<i64>> pascal(int n, i64 mod) {
    std::vector<std::vector<i64>> c(n+1, std::vector<i64>(n+1, 0));
    for (int i = 0; i <= n; ++i) {
        c[i][0] = 1;
        for (int j = 1; j <= i; ++j)
            c[i][j] = (c[i-1][j-1] + c[i-1][j]) % mod;
    }
    return c;
}

int main() {
    Combinatorics comb(200000);

    // Known values.
    assert(comb.C(5, 2) == 10);
    assert(comb.C(10, 3) == 120);
    assert(comb.C(0, 0) == 1);
    assert(comb.C(5, 0) == 1);
    assert(comb.C(5, 6) == 0);   // k > n -> 0

    // C(20, 10) = 184756.
    assert(comb.C(20, 10) == 184756);

    // Small Pascal table.
    auto p = pascal(6, MOD);
    assert(p[6][3] == 20);        // C(6,3) = 20
    assert(p[5][2] == 10);        // C(5,2) = 10

    // C(1000, 500) mod p: only check range consistency.
    i64 v = comb.C(1000, 500);
    assert(v > 0 && v < MOD);

    std::print("03-combinatorics-mod: all assertions passed\n");
    return 0;
}
