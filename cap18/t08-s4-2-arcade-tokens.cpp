// Exercise T08.6 - The Arcade Tokens
// Session: T08.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 08 (coin change, counting-combinations variant)
// Tags: chapter-08, dynamic-programming
//
// MODEL: count multisets of denominations summing to t - coin change
//   variant 2 in the chapter's taxonomy. The crux is the loop order:
//   it decides whether order matters, and the two answers differ
//   wildly (compositions vs partitions).
// MATH: process denominations one at a time, outer loop over coins,
//   inner over amounts ascending: dp[a] after processing the first k
//   denominations = number of multisets using only those k that sum
//   to a; dp[0] = 1 (the empty purchase). Transition for coin d:
//   dp[a] += dp[a - d], amounts ascending (unbounded: a coin may be
//   reused within its own pass).
// PROOF: a multiset over the first k denominations summing to a
//   either uses no coin d_k (counted by the previous pass) or
//   contains at least one d_k - removing exactly one yields a multiset
//   summing to a - d that may still contain d_k (counted earlier in
//   THIS pass, ascending order). The map "remove one d_k" is a
//   bijection between the second class and the multisets for a - d, so
//   nothing is double-counted. Swapping the loops (amounts outer,
//   coins inner) would instead count ordered sequences: 1+2+2, 2+1+2
//   and 2+2+1 separately - the classic interview wound, here
//   inflicted deliberately once so it never happens in a contest.
// COMPLEXITY: O(nt) time (1e7 at the limits), O(t) memory. One pass
//   per denomination; no per-coin copies of the table.
// TYPES: counts are reduced mod 1e9+7 and stored in long long;
//   dp[a] + dp[a-d] peaks below 2e9+14, which overflows int but not
//   long long (and would even fit unsigned int - too clever to be
//   worth the comment explaining it).
// ALTERNATIVES: generating functions: the answer is [x^t] of the
//   product over d of 1/(1 - x^d); the DP is exactly that product
//   evaluated coefficient by coefficient, one factor per pass. The
//   composition count (order matters) is variant 3, one loop swap
//   away; the min-coin count is variant 1 (the Tram Fare, T08.3).
// EDGE CASES: t smaller than every denomination (0); t = some d
//   (counts the singleton); single denomination d (1 if d divides t,
//   else 0); answer needing the modulus (large t, denomination 1
//   present makes counts explode combinatorially).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tokens t08-s4-2-arcade-tokens.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetokens.exe t08-s4-2-arcade-tokens.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int target = 0;
    std::cin >> n >> target;

    std::vector<int> denom(static_cast<std::size_t>(n));
    for (auto& d : denom) {
        std::cin >> d;
    }

    constexpr long long kMod = 1'000'000'007;
    std::vector<long long> dp(static_cast<std::size_t>(target) + 1, 0);
    dp[0] = 1;
    for (const int d : denom) {           // coins outer: multisets
        for (int a = d; a <= target; ++a) {  // amounts ascending: unbounded
            dp[static_cast<std::size_t>(a)] =
                (dp[static_cast<std::size_t>(a)] + dp[static_cast<std::size_t>(a - d)]) % kMod;
        }
    }
    std::print("{}\n", dp[static_cast<std::size_t>(target)]);
    return 0;
}
