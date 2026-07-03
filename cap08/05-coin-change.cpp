// cap08/05-coin-change.cpp
// Coin Change: minimum coins and counting combinations/sequences.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o coin_change 05-coin-change.cpp

#include <algorithm>
#include <cassert>
#include <limits>
#include <print>
#include <vector>

using i64 = long long;
constexpr int INF = std::numeric_limits<int>::max() / 2;
constexpr i64 MOD = 1'000'000'007;

// ============================================================
// Brute force: expands all coin combinations.
// Complexity: O(k^(W/min_coin)) -- infeasible for large W.
// Used only to validate the DP solution on small tests.
// ============================================================
int coin_brute(const std::vector<int>& coins, int amount, int idx = 0) {
    if (amount == 0) return 0;                                    // found a combination
    if (amount < 0 || idx == static_cast<int>(coins.size()))
        return INF;                                               // impossible
    // Two options: use coins[idx] again (idx does not advance), or skip it.
    int use  = coin_brute(coins, amount - coins[idx], idx);
    int skip = coin_brute(coins, amount, idx + 1);
    if (use != INF) use += 1;
    return std::min(use, skip);
}

// ============================================================
// Minimum coins to form exactly W -- O(n * W).
// dp[w] = minimum coins needed to form w.
// Relation: dp[w] = min(dp[w - c] + 1) for c in coins, c <= w.
// Loop order is not the semantic distinction here, unlike counting variants.
// ============================================================
int coin_min(const std::vector<int>& coins, int W) {
    std::vector<int> dp(W + 1, INF);
    dp[0] = 0;                                                    // base: 0 coins to form 0
    for (int w = 1; w <= W; ++w)
        for (int c : coins)
            if (c <= w && dp[w - c] != INF)
                dp[w] = std::min(dp[w], dp[w - c] + 1);
    return dp[W] == INF ? -1 : dp[W];
}

// ============================================================
// Counts combinations (order does not matter) -- O(n * W).
// Each multiset counts once: {1,2} == {2,1}.
//
// Key: outer loop over coins.
// When processing coin c, dp[w] accumulates only combinations that use
// already processed coins, so each multiset is counted once.
// ============================================================
i64 coin_combinations(const std::vector<int>& coins, int W) {
    std::vector<i64> dp(W + 1, 0);
    dp[0] = 1;                                                    // one way to form 0: empty set
    for (int c : coins)                                           // outer loop over coins
        for (int w = c; w <= W; ++w)
            dp[w] = (dp[w] + dp[w - c]) % MOD;
    return dp[W];
}

// ============================================================
// Counts ordered sequences (compositions) -- O(n * W).
// {1,2} and {2,1} are distinct sequences.
//
// Key: outer loop over W.
// For each amount w, any coin c may be the last coin used.
// ============================================================
i64 coin_sequences(const std::vector<int>& coins, int W) {
    std::vector<i64> dp(W + 1, 0);
    dp[0] = 1;
    for (int w = 1; w <= W; ++w)                                  // outer loop over values
        for (int c : coins)
            if (c <= w)
                dp[w] = (dp[w] + dp[w - c]) % MOD;
    return dp[W];
}

int main() {
    const std::vector<int> coins_us = {1, 5, 10, 25};            // US coins

    // --- minimum coins ---
    assert(coin_min(coins_us, 0)  == 0);
    assert(coin_min(coins_us, 1)  == 1);
    assert(coin_min(coins_us, 11) == 2);                          // 10 + 1
    assert(coin_min(coins_us, 30) == 2);                          // 25 + 5
    assert(coin_min(coins_us, 41) == 4);                          // 25 + 10 + 5 + 1
    assert(coin_min({2}, 3)       == -1);                         // impossible

    // Brute-force vs DP consistency for W <= 40.
    for (int w = 0; w <= 40; ++w) {
        int bf = coin_brute(coins_us, w);
        int dp = coin_min(coins_us, w);
        assert((bf == INF ? -1 : bf) == dp);
    }

    // --- combination count {1,2}, W=4: {4x1, 1+1+2, 2+2} = 3 ---
    assert(coin_combinations({1, 2}, 4) == 3);
    std::print("coin_combinations({{1,5,10,25}}, 200) = {}\n",
               coin_combinations(coins_us, 200));

    // --- sequence count {1,2}, W=3: (1+1+1),(1+2),(2+1) = 3 ---
    assert(coin_sequences({1, 2}, 3) == 3);
    // {1,2}, W=4: (1+1+1+1),(1+1+2),(1+2+1),(2+1+1),(2+2) = 5
    assert(coin_sequences({1, 2}, 4) == 5);

    std::print("coin_min({{1,5,10,25}}, 36) = {}\n",  coin_min(coins_us, 36));
    std::print("coin_sequences({{1,2,3}}, 4) = {}\n", coin_sequences({1, 2, 3}, 4));
    std::print("05-coin-change: all assertions passed.\n");
    return 0;
}
