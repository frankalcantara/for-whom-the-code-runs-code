// Exercise A6.4 - The Caravan Passes
// Session: A6, exercise 4 of 8 (6 hours for the set)
// Theme: chapter 09 revision (Dijkstra with a lazy-deletion binary
//   heap), with chapter 06's priority_queue underneath
// Tags: chapter-09, chapter-06, dijkstra
//
// MODEL: single-source shortest path on an undirected graph with
//   positive weights.
// MATH: Dijkstra - repeatedly settle the unsettled vertex of
//   minimum tentative distance, relax its edges. The heap holds
//   (distance, vertex) pairs; stale pairs (distance greater than
//   the recorded one) are skipped on pop instead of being decreased
//   in place - the lazy-deletion idiom, since std::priority_queue
//   has no decrease-key.
// PROOF: the classic cut argument, valid because weights are
//   positive: when (d, v) with d == dist[v] is popped, any other
//   route to v would leave the settled set through some unsettled
//   u with dist[u] >= d and gain a non-negative tail - no shorter
//   route exists. Induction over pops; stale pairs change nothing
//   (their vertex is already settled with a smaller value).
// COMPLEXITY: O((n + m) log m) with up to m heap entries; memory
//   O(n + m). At n = 2e5, m = 4e5: well inside limits.
// TYPES: the distance trap - n * w reaches 2e5 * 1e9 = 2e14:
//   long long for distances and the infinity sentinel; int is an
//   instant wrong answer on long thin graphs. Vertices fit int32.
// ALTERNATIVES: Bellman-Ford O(nm) = 8e10, dead, and there are no
//   negative edges to justify it; 0-1 BFS needs weights in {0,1};
//   bidirectional Dijkstra halves the searched ball for one query
//   but doubles the code - not worth it here.
// EDGE CASES: n unreachable (-1); multiple edges between the same
//   posts (relaxation keeps the best - no dedup needed); self
//   loops (never improve, harmless); m = 0 (-1 for n > 1); the
//   2e14 distance ceiling exercised by a path graph at the limits.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o caravan a6-04-caravan-passes.cpp

#include <iostream>
#include <print>
#include <queue>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<std::pair<int, long long>>> adj(n + 1);
    for (std::size_t i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        long long w = 0;
        std::cin >> u >> v >> w;
        adj[static_cast<std::size_t>(u)].emplace_back(v, w);
        adj[static_cast<std::size_t>(v)].emplace_back(u, w);
    }

    constexpr long long kInf = 4e18;
    std::vector<long long> dist(n + 1, kInf);
    using Entry = std::pair<long long, int>;  // (distance, vertex)
    std::priority_queue<Entry, std::vector<Entry>, std::greater<>> heap;
    dist[1] = 0;
    heap.emplace(0, 1);
    while (!heap.empty()) {
        const auto [d, v] = heap.top();
        heap.pop();
        const auto uv = static_cast<std::size_t>(v);
        if (d > dist[uv]) continue;  // stale entry: lazy deletion
        for (const auto& [to, w] : adj[uv]) {
            const auto ut = static_cast<std::size_t>(to);
            if (d + w < dist[ut]) {
                dist[ut] = d + w;
                heap.emplace(dist[ut], to);
            }
        }
    }

    std::print("{}\n", dist[n] == kInf ? -1 : dist[n]);
    return 0;
}
