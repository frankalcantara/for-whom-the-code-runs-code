// Exercise T14.1 - The Ironmonger's Bundles
// Session: T14.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 14 (minimum set cover by bitmask DP)
// Tags: chapter-14, dynamic-programming, bitmask-dp, bitmask, brute-force
//
// MODEL: minimum set cover over a universe of n <= 20 elements -
//   NP-hard in general, an instruction to enumerate subsets here.
// MATH: encode tool sets as 20-bit masks. dp[s] = fewest bundles
//   whose union is a SUPERSET of s... stored equivalently as: for
//   each reachable union u, dp[u] = fewest bundles achieving
//   exactly union u; sweep dp in increasing mask order, extending
//   every state by every bundle: dp[u | bundle_j] <-
//   min(dp[u] + 1). Answer dp[full].
// PROOF: induction on the number of bundles used - any optimal
//   cover of k bundles, processed in any order, passes through
//   intermediate unions each of which the sweep has assigned a
//   value <= its position in the sequence (the sweep tries ALL
//   extensions of ALL states, in an order compatible with mask
//   growth since u | b >= u). Hence dp[full] <= k; >= holds
//   because every dp transition corresponds to an actual bundle
//   purchase.
// COMPLEXITY: O(2^n * m) = 1e6 * 50 = 5e7 cheap operations,
//   O(2^n) memory (4 MB of int32). The chapter's bread and
//   butter: exponential in n, and n was chosen so that this is
//   fine.
// TYPES: masks in uint32 (20 bits); dp values <= m + 1 fit int8
//   but int32 keeps the code unremarkable; no overflow anywhere.
// ALTERNATIVES: greedy largest-remaining-coverage - ln(n)
//   approximation, can be off by a factor on adversarial bundles
//   (and the exam wants the minimum, not an estimate); branch and
//   bound - works, unbounded worst case; inclusion-exclusion
//   counting covers - answers "how many", not "how few".
// EDGE CASES: a tool in no bundle (-1 - dp[full] stays infinite);
//   one bundle covering everything (1); bundles that duplicate
//   each other (harmless - same transitions); n = 1; the full
//   m = 50 with disjoint singletons (answer 20, the slow diagonal
//   of the DP).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o bundles t14-s1-1-ironmongers-bundles.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;

    std::vector<std::uint32_t> bundle(m, 0);
    for (std::size_t i = 0; i < m; ++i) {
        std::size_t count = 0;
        std::cin >> count;
        for (std::size_t j = 0; j < count; ++j) {
            unsigned tool = 0;
            std::cin >> tool;
            bundle[i] |= std::uint32_t{1} << (tool - 1);
        }
    }

    const std::uint32_t full = (std::uint32_t{1} << n) - 1;
    constexpr std::int32_t kInf = 1 << 28;
    std::vector<std::int32_t> dp(static_cast<std::size_t>(full) + 1, kInf);
    dp[0] = 0;
    for (std::uint32_t u = 0; u <= full; ++u) {
        if (dp[u] == kInf) continue;
        for (std::size_t j = 0; j < m; ++j) {
            const std::uint32_t next = u | bundle[j];
            if (dp[u] + 1 < dp[next]) dp[next] = dp[u] + 1;
        }
    }
    std::print("{}\n", dp[full] == kInf ? -1 : dp[full]);
    return 0;
}
