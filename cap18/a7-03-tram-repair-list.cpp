// Exercise A7.3 - The Tram Repair List
// Session: A7, exercise 3 of 9
// Theme: integration of chapter 09 (Dijkstra) and chapter 14
//   (bitmask DP over required stops)
// Tags: chapter-09, chapter-14, dijkstra, dynamic-programming, bitmask-dp, bitmask, brute-force
//
// MODEL: shortest walk from stop 1 that visits a small set of
//   required stops in a large weighted graph.
// MATH: run Dijkstra from stop 1 and from each required stop. This
//   compresses the large graph to distances between special stops.
//   Then dp[mask][last] is the shortest length that has visited the
//   required stops in mask and currently ends at required stop last.
//   Transitions append one unvisited required stop using the
//   precomputed shortest-path distance.
// PROOF: Dijkstra gives exact shortest distances between every pair
//   of special stops because all edge weights are positive. Any
//   feasible route induces an order in which required stops are
//   first completed; replacing each segment between consecutive
//   special stops by a shortest path cannot make the route longer.
//   The subset DP enumerates all such orders, so its minimum is
//   optimal.
// COMPLEXITY: O((k + 1)(m log n) + 2^k k^2) time and
//   O(n + m + 2^k k) memory.
// TYPES: path lengths can reach about 3e14, so long long is used.
//   The infinity sentinel is chosen far below signed overflow.
// ALTERNATIVES: Floyd-Warshall is impossible at n = 2e5; TSP DP on
//   all graph vertices is nonsense; a greedy nearest required stop
//   can fail because local closeness is not an order proof.
// EDGE CASES: unreachable required stop (-1); k = 1; required stop
//   equal to stop 1; parallel tracks; routes that pass through
//   required stops before their turn are still valid because the DP
//   optimizes over completion order, not physical taboo.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tram a7-03-tram-repair-list.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetram.exe a7-03-tram-repair-list.cpp

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <utility>
#include <vector>

namespace {

constexpr long long kInf = 4'000'000'000'000'000'000LL;

std::vector<long long> dijkstra(
    int source,
    const std::vector<std::vector<std::pair<int, int>>>& graph) {
    std::vector<long long> dist(graph.size(), kInf);
    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[source] = 0;
    pq.emplace(0, source);
    while (!pq.empty()) {
        const auto [d, v] = pq.top();
        pq.pop();
        if (d != dist[v]) continue;
        for (const auto [to, weight] : graph[v]) {
            if (dist[to] > d + weight) {
                dist[to] = d + weight;
                pq.emplace(dist[to], to);
            }
        }
    }
    return dist;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    int k = 0;
    std::cin >> n >> m >> k;
    std::vector<int> required(k);
    for (int& stop : required) {
        std::cin >> stop;
        --stop;
    }

    std::vector<std::vector<std::pair<int, int>>> graph(n);
    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        int w = 0;
        std::cin >> u >> v >> w;
        --u;
        --v;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    std::vector<std::vector<long long>> special_dist;
    special_dist.push_back(dijkstra(0, graph));
    for (const int stop : required) {
        special_dist.push_back(dijkstra(stop, graph));
    }

    const int states = 1 << k;
    std::vector<std::vector<long long>> dp(
        states, std::vector<long long>(k, kInf));
    for (int i = 0; i < k; ++i) {
        dp[1 << i][i] = special_dist[0][required[i]];
    }

    for (int mask = 0; mask < states; ++mask) {
        for (int last = 0; last < k; ++last) {
            if (dp[mask][last] == kInf) continue;
            for (int next = 0; next < k; ++next) {
                if ((mask >> next) & 1) continue;
                const long long step = special_dist[last + 1][required[next]];
                if (step == kInf) continue;
                const int next_mask = mask | (1 << next);
                dp[next_mask][next] =
                    std::min(dp[next_mask][next], dp[mask][last] + step);
            }
        }
    }

    const int full = states - 1;
    long long answer = kInf;
    for (int last = 0; last < k; ++last) {
        answer = std::min(answer, dp[full][last]);
    }
    std::print("{}\n", answer == kInf ? -1 : answer);
    return 0;
}
