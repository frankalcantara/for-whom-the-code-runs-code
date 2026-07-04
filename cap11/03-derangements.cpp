// cap11/03-derangements.cpp
// Solved exercise: derangements and permutations with exactly k fixed points.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o derangements 03-derangements.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Federangements.exe 03-derangements.cpp

#include <cassert>
#include <print>
#include <vector>

using i64 = long long;
constexpr i64 MOD = 1000000007;

// Number of derangements D(n): permutations with no fixed points.
// D(0)=1, D(1)=0, D(n)=(n-1)*(D(n-1)+D(n-2)).
std::vector<i64> derangements(int n) {
    std::vector<i64> d(n + 1);
    d[0] = 1;
    if (n == 0) return d;
    d[1] = 0;
    for (int i = 2; i <= n; ++i)
        d[i] = (i - 1) % MOD * ((d[i - 1] + d[i - 2]) % MOD) % MOD;
    return d;
}

static i64 pow_mod(i64 b, i64 e, i64 m = MOD) {
    i64 r = 1;
    b %= m;
    while (e > 0) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

i64 perms_with_k_fixed(int n, int k, const std::vector<i64>& d,
                       const std::vector<i64>& fat,
                       const std::vector<i64>& inv_fat) {
    if (k > n) return 0;
    i64 comb_nk = fat[n] % MOD * inv_fat[k] % MOD * inv_fat[n - k] % MOD;
    return comb_nk * d[n - k] % MOD;
}

int main() {
    auto d = derangements(10);

    assert(d[0] == 1);
    assert(d[1] == 0);
    assert(d[2] == 1);
    assert(d[3] == 2);
    assert(d[4] == 9);
    assert(d[5] == 44);
    assert(d[6] == 265);
    assert(d[7] == 1854);
    assert(d[8] == 14833);
    assert(d[9] == 133496);
    assert(d[10] == 1334961);

    std::vector<i64> fat(11), inv_fat(11);
    fat[0] = 1;
    for (int i = 1; i <= 10; ++i)
        fat[i] = fat[i - 1] * i % MOD;
    inv_fat[10] = pow_mod(fat[10], MOD - 2);
    for (int i = 9; i >= 0; --i)
        inv_fat[i] = inv_fat[i + 1] * (i + 1) % MOD;

    assert(perms_with_k_fixed(5, 0, d, fat, inv_fat) == 44);
    assert(perms_with_k_fixed(5, 1, d, fat, inv_fat) == 45);
    assert(perms_with_k_fixed(5, 5, d, fat, inv_fat) == 1);

    i64 total = 0;
    for (int k = 0; k <= 5; ++k)
        total = (total + perms_with_k_fixed(5, k, d, fat, inv_fat)) % MOD;
    assert(total == 120);

    std::print("03-derangements: all assertions passed\n");
    return 0;
}
