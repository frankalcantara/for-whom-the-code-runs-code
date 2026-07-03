// cap10/04-counting-representations.cpp
// Solved exercise: count representations of n using values in {1,2,...,k},
// modulo 10^9+7.
//
// This is counting unbounded knapsack with modular arithmetic.
// dp[i] = number of ways to sum to i with values in {1..k}.
// dp[0] = 1 (empty sum); dp[i] += dp[i - c] for each value c.
//
// The loop order decides the combinatorial object:
//   - Ordered compositions: outer loop over sum i, inner loop over value c.
//   - Unordered partitions: outer loop over value c, inner loop over sum i.
//
// Stars and bars is a separate problem:
// x1 + x2 + ... + xk = n with xi >= 0, where the k boxes are labeled.
// The answer is C(n + k - 1, k - 1).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o counting 04-counting-representations.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <vector>

using i64 = long long;
constexpr i64 MOD = 1000000007;

// ============================================================
// Count compositions: ordered sequences that sum to n
// using values from {1, 2, ..., k} with repetition.
// dp[i] = number of compositions of i.
// Recurrence: dp[i] = sum_{c=1}^{min(i,k)} dp[i-c].
// O(n * k) time, O(n) memory.
// ============================================================
i64 count_compositions(int n, int k) {
    std::vector<i64> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int c = 1; c <= std::min(i, k); ++c)
            dp[i] = (dp[i] + dp[i - c]) % MOD;
    return dp[n];
}

// ============================================================
// Count partitions: unordered multisets that sum to n
// using values from {1, 2, ..., k} with repetition.
// The outer loop over values enforces nondecreasing construction.
// O(n * k) time, O(n) memory.
// ============================================================
i64 count_partitions(int n, int k) {
    std::vector<i64> dp(n + 1, 0);
    dp[0] = 1;
    for (int c = 1; c <= k; ++c)
        for (int i = c; i <= n; ++i)
            dp[i] = (dp[i] + dp[i - c]) % MOD;
    return dp[n];
}

// ============================================================
// Stars and bars: C(n + k - 1, k - 1) mod p
// Number of nonnegative integer k-tuples that sum to n.
// This is not the same as coin partitions by values.
// ============================================================
static i64 pow_mod(i64 base, i64 exp, i64 mod) {
    i64 r = 1; base %= mod;
    while (exp > 0) {
        if (exp & 1) r = r * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return r;
}

i64 stars_and_bars(int n, int k) {
    // C(n + k - 1, k - 1): factorial table up to n+k.
    int N = n + k;
    std::vector<i64> fat(N + 1);
    fat[0] = 1;
    for (int i = 1; i <= N; ++i) fat[i] = fat[i-1] * i % MOD;
    auto inv = [&](i64 x) { return pow_mod(x, MOD - 2, MOD); };
    return fat[n + k - 1] % MOD * inv(fat[k-1]) % MOD * inv(fat[n]) % MOD;
}

int main() {
    // Compositions of 4 with values {1,2}:
    // {1+1+1+1, 1+1+2, 1+2+1, 2+1+1, 2+2} = 5.
    assert(count_compositions(4, 2) == 5);

    // Compositions of 3 with values {1,2,3}: {3, 1+2, 2+1, 1+1+1} = 4.
    assert(count_compositions(3, 3) == 4);

    // Partitions of 4 with values {1,2}: {1+1+1+1, 1+1+2, 2+2} = 3.
    assert(count_partitions(4, 2) == 3);

    // Partitions of 5 with values up to 5: the integer partition number p(5) = 7.
    assert(count_partitions(5, 5) == 7);

    // stars and bars: C(4+3-1, 3-1) = C(6,2) = 15
    assert(stars_and_bars(4, 3) == 15);

    // stars and bars: C(0+3-1, 2) = C(2,2) = 1 (only {0,0,0})
    assert(stars_and_bars(0, 3) == 1);

    std::print("04-counting-representations: all assertions passed\n");
    return 0;
}
