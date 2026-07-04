// cap11/01-counting.cpp
// Permutations, combinations, inclusion-exclusion, Catalan numbers, and Stirling numbers.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o counting 01-counting.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecounting.exe 01-counting.cpp

#include <cassert>
#include <numeric>
#include <print>
#include <vector>

using i64 = long long;
constexpr i64 MOD = 1000000007;

static i64 pow_mod(i64 base, i64 exp, i64 mod = MOD) {
    i64 r = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) r = r * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return r;
}

struct Comb {
    int n;
    std::vector<i64> fat, inv_fat;

    explicit Comb(int n_) : n(n_), fat(n_ + 1), inv_fat(n_ + 1) {
        fat[0] = 1;
        for (int i = 1; i <= n_; ++i)
            fat[i] = fat[i - 1] * i % MOD;

        inv_fat[n_] = pow_mod(fat[n_], MOD - 2);
        for (int i = n_ - 1; i >= 0; --i)
            inv_fat[i] = inv_fat[i + 1] * (i + 1) % MOD;
    }

    // C(a, b) modulo MOD: choose b elements from a, order does not matter.
    i64 C(int a, int b) const {
        if (b < 0 || b > a) return 0;
        return fat[a] % MOD * inv_fat[b] % MOD * inv_fat[a - b] % MOD;
    }

    // P(a, b) modulo MOD: ordered selection of b elements from a.
    i64 P(int a, int b) const {
        if (b < 0 || b > a) return 0;
        return fat[a] % MOD * inv_fat[a - b] % MOD;
    }
};

// Inclusion-exclusion for integers in [1, n] divisible by at least one divisor.
// Enumerates nonempty subsets of divisors: O(2^k * k).
i64 pie_divisible(i64 n, const std::vector<i64>& divisors) {
    int k = static_cast<int>(divisors.size());
    i64 result = 0;
    for (int mask = 1; mask < (1 << k); ++mask) {
        i64 lcm_val = 1;
        int bits = 0;
        for (int i = 0; i < k; ++i) {
            if ((mask >> i) & 1) {
                lcm_val = lcm_val / std::gcd(lcm_val, divisors[i]) * divisors[i];
                if (lcm_val > n) { lcm_val = n + 1; break; }
                ++bits;
            }
        }
        i64 term = n / lcm_val;
        if (bits % 2 == 1) result += term;
        else               result -= term;
    }
    return result;
}

// Catalan number: C_n = C(2n, n) / (n + 1), modulo prime MOD.
i64 catalan(int n, const Comb& comb) {
    return comb.C(2 * n, n) % MOD * pow_mod(n + 1, MOD - 2) % MOD;
}

// Stirling numbers of the second kind.
// S(n, k) counts partitions of n labeled objects into k nonempty unlabeled blocks.
std::vector<std::vector<i64>> stirling2(int n) {
    std::vector<std::vector<i64>> s(n + 1, std::vector<i64>(n + 1, 0));
    s[0][0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= i; ++j)
            s[i][j] = (j * s[i - 1][j] % MOD + s[i - 1][j - 1]) % MOD;
    return s;
}

int main() {
    Comb comb(200);

    assert(comb.C(5, 2) == 10);
    assert(comb.C(10, 3) == 120);
    assert(comb.C(0, 0) == 1);
    assert(comb.C(5, 6) == 0);

    assert(comb.P(5, 2) == 20);   // 5 * 4
    assert(comb.P(4, 4) == 24);   // 4!

    // Stars and bars: x1+x2+x3=4, xi>=0 -> C(6,2)=15.
    assert(comb.C(4 + 3 - 1, 3 - 1) == 15);

    // Integers in [1,100] divisible by 2, 3, or 5.
    assert(pie_divisible(100, {2, 3, 5}) == 74);
    assert(pie_divisible(21, {3, 7}) == 9);

    assert(catalan(0, comb) == 1);
    assert(catalan(1, comb) == 1);
    assert(catalan(2, comb) == 2);
    assert(catalan(3, comb) == 5);
    assert(catalan(4, comb) == 14);
    assert(catalan(5, comb) == 42);

    auto s = stirling2(6);
    assert(s[1][1] == 1);
    assert(s[3][2] == 3);
    assert(s[4][2] == 7);
    assert(s[6][3] == 90);

    std::print("01-counting: all assertions passed\n");
    return 0;
}
