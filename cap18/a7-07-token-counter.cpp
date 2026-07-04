// Exercise A7.7 - The Token Counter
// Session: A7, exercise 7 of 9
// Theme: chapter 08 (coin-change combinations)
// Tags: chapter-08, dynamic-programming
//
// MODEL: count unordered multisets of unlimited coins that sum to T.
// MATH: dp[s] is the number of multisets using only coin values
//   processed so far and summing to s. Processing coins outside and
//   sums upward allows the current coin to be reused while keeping a
//   canonical nondecreasing coin order.
// PROOF: induction over coin types. Before any coin, only sum 0 has
//   one representation. When coin c is processed, dp[s - c] already
//   counts multisets using coins up to c, so appending c creates
//   exactly the multisets whose largest processed coin instance is c.
//   No ordering duplicates are created because coin types are never
//   revisited.
// COMPLEXITY: O(nT) time and O(T) memory.
// TYPES: values are reduced modulo 1e9+7 after each addition, so int
//   is safe for table entries when the addition is performed in int.
// ALTERNATIVES: amount-outside loops count ordered sequences, which
//   is a different problem; recursion with memoization uses the same
//   state but more overhead.
// EDGE CASES: T equal to one coin; coin value 1; no way to form T;
//   repeated coin values, which are treated as separate listed types
//   by the input order.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tokens a7-07-token-counter.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetokens.exe a7-07-token-counter.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int kMod = 1'000'000'007;
    int n = 0;
    int target = 0;
    std::cin >> n >> target;
    std::vector<int> coins(n);
    for (int& coin : coins) std::cin >> coin;

    std::vector<int> dp(target + 1, 0);
    dp[0] = 1;
    for (const int coin : coins) {
        for (int sum = coin; sum <= target; ++sum) {
            dp[sum] += dp[sum - coin];
            if (dp[sum] >= kMod) dp[sum] -= kMod;
        }
    }

    std::print("{}\n", dp[target]);
    return 0;
}
