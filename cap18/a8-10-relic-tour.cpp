// Exercise A8.10 - The Relic Tour
// Session: A8, exercise 10 of 10
// Theme: chapter 14 (Held-Karp bitmask DP for TSP)
// Tags: chapter-14, dynamic-programming, bitmask-dp, bitmask
//
// MODEL: minimum Hamiltonian cycle starting and ending at room 0.
// MATH: dp[mask][v] is the minimum cost to start at 0, visit
//   exactly mask, and end at v. Extend to each unvisited room.
// PROOF: induction on popcount(mask). Every optimal state path has
//   a previous endpoint before v; the recurrence tries all such
//   predecessors through forward extensions and keeps the minimum.
// COMPLEXITY: O(2^n n^2) time and O(2^n n) memory.
// TYPES: tour cost is at most 16e9, so int64_t is used.
// ALTERNATIVES: permutations are O(n!); approximation is not exact.
// EDGE CASES: n=2; zero-cost edges; asymmetric costs are not
//   required but the DP would still handle them.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o tour a8-10-relic-tour.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<std::vector<std::int64_t>> cost(n, std::vector<std::int64_t>(n));
    for (auto& row : cost) {
        for (auto& value : row) std::cin >> value;
    }
    constexpr std::int64_t kInf = 4'000'000'000'000'000'000LL;
    const int states = 1 << n;
    std::vector<std::vector<std::int64_t>> dp(
        states, std::vector<std::int64_t>(n, kInf));
    dp[1][0] = 0;
    for (int mask = 1; mask < states; ++mask) {
        if ((mask & 1) == 0) continue;
        for (int v = 0; v < n; ++v) {
            if (dp[mask][v] == kInf) continue;
            for (int u = 0; u < n; ++u) {
                if ((mask >> u) & 1) continue;
                const int next = mask | (1 << u);
                dp[next][u] = std::min(dp[next][u], dp[mask][v] + cost[v][u]);
            }
        }
    }
    std::int64_t answer = kInf;
    for (int v = 1; v < n; ++v) {
        answer = std::min(answer, dp[states - 1][v] + cost[v][0]);
    }
    std::print("{}\n", answer);
    return 0;
}
