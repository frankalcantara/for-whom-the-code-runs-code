// Exercise T14.2 - The Grinder's Circuit
// Session: T14.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 14 (Held-Karp bitmask DP for TSP)
// Tags: chapter-14, dynamic-programming, bitmask-dp, bitmask
//
// MODEL: shortest Hamiltonian cycle from vertex 0 - the traveling
//   salesman, exact, because n <= 17 says so.
// MATH: dp[mask][v] = length of the shortest path that starts at
//   0, visits exactly the vertices of mask (0 and v included), and
//   ends at v. Transition: extend to any u not in mask:
//   dp[mask | 1<<u][u] <- dp[mask][v] + d[v][u]. Answer:
//   min over v != 0 of dp[full][v] + d[v][0].
// PROOF: induction on popcount(mask) - every path ending at v
//   with vertex set mask decomposes uniquely into a path ending
//   at its second-to-last vertex w (set mask \ {v}) plus edge
//   (w, v); the DP takes the min over all w, so dp holds the true
//   optimum per state. The final closing edge converts the best
//   path into the best tour; tours decompose the same way.
// COMPLEXITY: O(2^n * n^2) = 1.3e5 * 289 ~ 3.8e7 transitions at
//   n = 17; memory O(2^n * n) int32 = 8.9 MB. Both chosen by the
//   constraint-setter with this exact algorithm in mind.
// TYPES: tour length <= 17 * 1e6 < 2^31: int32 suffices and is
//   what the dp table stores (memory matters at 2^17 * 17
//   entries); the infinity sentinel must survive one addition -
//   kInf = 2^29 keeps kInf + d below int32 overflow.
// ALTERNATIVES: brute force (n-1)! permutations - 2e13 at n = 17,
//   dead; branch and bound with a lower-bound heuristic - often
//   fast, never guaranteed; the metric 2-approximation via MST -
//   wrong tool when exactness is demanded and n is this small.
// EDGE CASES: n = 2 (out and back: 2 * d01); zero-length edges
//   (allowed - distances of 0 between distinct villages do not
//   break the DP); the asymmetry guard unnecessary (matrix is
//   symmetric by contract, but the DP never assumes it); maximum
//   n with maximum distances (the int32 ceiling argument above).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o circuit t14-s2-1-grinders-circuit.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;
    std::vector<std::vector<std::int32_t>> d(n, std::vector<std::int32_t>(n));
    for (auto& row : d) {
        for (auto& v : row) std::cin >> v;
    }

    constexpr std::int32_t kInf = 1 << 29;
    const std::size_t full = (std::size_t{1} << n) - 1;
    std::vector<std::vector<std::int32_t>> dp(
        full + 1, std::vector<std::int32_t>(n, kInf));
    dp[1][0] = 0;  // at home, nothing visited but home

    for (std::size_t mask = 1; mask <= full; ++mask) {
        if ((mask & 1) == 0) continue;  // tours start at village 0
        for (std::size_t v = 0; v < n; ++v) {
            const std::int32_t cur = dp[mask][v];
            if (cur >= kInf || (mask >> v & 1) == 0) continue;
            for (std::size_t u = 0; u < n; ++u) {
                if (mask >> u & 1) continue;
                const std::size_t next = mask | (std::size_t{1} << u);
                dp[next][u] = std::min(dp[next][u], cur + d[v][u]);
            }
        }
    }

    std::int32_t best = kInf;
    for (std::size_t v = 1; v < n; ++v) {
        best = std::min(best, dp[full][v] + d[v][0]);
    }
    std::print("{}\n", best);
    return 0;
}
