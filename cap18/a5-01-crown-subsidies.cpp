// Exercise A5.1 - The Crown Subsidies
// Session: A5, exercise 1 of 7 (6 hours for the evaluation)
// Theme: chapter 09 (Bellman-Ford with negative edges; negative-cycle
//   detection restricted to cycles that actually matter)
// Tags: chapter-09, greedy, bfs, dijkstra
//
// MODEL: a directed graph with arbitrary integer edge weights; the trip
//   is a shortest walk from 1 to n. With negative edges Dijkstra's
//   greedy commitment is unsound; Bellman-Ford relaxes every edge
//   n - 1 times, which suffices for any simple path. The answer is
//   "unbounded" exactly when some negative cycle lies on a walk from
//   1 to n - reachable from 1 AND able to reach n. A negative cycle
//   elsewhere in the graph is scenery, not a refund.
// MATH: dist[v] after round i is at most the weight of any walk from
//   1 to v using at most i edges. A shortest walk that is not
//   improvable by a negative cycle can be taken simple, hence uses at
//   most n - 1 edges. If round n still relaxes some edge (u, v), then
//   v sits on or behind a negative cycle reachable from 1; the trip
//   cost is unbounded iff n is reachable from such a v.
// PROOF: induction on rounds for the bound above (base: dist[1] = 0;
//   step: a best walk of <= i edges is a best walk of <= i-1 edges
//   plus one relaxed edge). For detection: if no edge relaxes in round
//   n, dist is a fixed point, and fixed points of the relaxation
//   operator are exactly the true shortest-walk distances (no
//   improving walk exists at all); if (u, v) relaxes in round n, the
//   walk to v can be shortened beyond every simple-path optimum, which
//   forces a repeated vertex, i.e. a negative cycle on the walk. BFS
//   from all such v decides whether the port inherits the discount.
// COMPLEXITY: O(nm) for Bellman-Ford = 2000 * 5000 = 1e7 relaxation
//   attempts, with early exit when a round changes nothing; O(n + m)
//   for the closing BFS. Memory O(n + m).
// TYPES: dist values are bounded by (n-1) * max|w| = 2e3 * 1e9 = 2e12
//   in magnitude across all useful relaxations, far outside int and
//   comfortably inside long long (chapter 01's first lesson, still
//   collecting rent). INF is a sentinel; relaxation is gated on
//   dist[u] != INF so the sentinel never participates in arithmetic.
// ALTERNATIVES: Dijkstra fails on negative edges (greedy finality is
//   the whole algorithm); Floyd-Warshall computes all pairs in O(n^3) =
//   8e9, too slow and answering a question nobody asked; SPFA is
//   Bellman-Ford with a queue and a worse worst case. Johnson's
//   reweighting needs Bellman-Ford anyway.
// EDGE CASES: n = 1 (the capital is the port: cost 0, no roads
//   needed); port unreachable; negative cycle reachable from 1 but
//   unable to reach n (must still print the finite optimum); negative
//   cycle on the route (unbounded); parallel roads with different
//   costs; m = 0.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o subsidies a5-01-crown-subsidies.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesubsidies.exe a5-01-crown-subsidies.cpp

#include <cstdint>
#include <iostream>
#include <limits>
#include <print>
#include <queue>
#include <vector>

namespace {

struct Road {
    int from;
    int to;
    std::int64_t cost;
};

constexpr std::int64_t kInf = std::numeric_limits<std::int64_t>::max();

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<Road> roads(static_cast<std::size_t>(m));
    std::vector<std::vector<int>> adjacency(static_cast<std::size_t>(n) + 1);
    for (auto& [from, to, cost] : roads) {
        std::cin >> from >> to >> cost;
        adjacency[static_cast<std::size_t>(from)].push_back(to);
    }

    std::vector<std::int64_t> dist(static_cast<std::size_t>(n) + 1, kInf);
    dist[1] = 0;

    for (int round = 1; round < n; ++round) {
        bool changed = false;
        for (const auto& [from, to, cost] : roads) {
            const auto u = static_cast<std::size_t>(from);
            const auto v = static_cast<std::size_t>(to);
            if (dist[u] != kInf && dist[u] + cost < dist[v]) {
                dist[v] = dist[u] + cost;
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }

    // Round n: any edge that still relaxes feeds a negative cycle
    // reachable from the capital. BFS forward from every such endpoint.
    std::vector<char> tainted(static_cast<std::size_t>(n) + 1, 0);
    std::queue<int> frontier;
    for (const auto& [from, to, cost] : roads) {
        const auto u = static_cast<std::size_t>(from);
        const auto v = static_cast<std::size_t>(to);
        if (dist[u] != kInf && dist[u] + cost < dist[v] && tainted[v] == 0) {
            tainted[v] = 1;
            frontier.push(to);
        }
    }
    while (!frontier.empty()) {
        const int u = frontier.front();
        frontier.pop();
        for (const int v : adjacency[static_cast<std::size_t>(u)]) {
            if (tainted[static_cast<std::size_t>(v)] == 0) {
                tainted[static_cast<std::size_t>(v)] = 1;
                frontier.push(v);
            }
        }
    }

    const auto port = static_cast<std::size_t>(n);
    if (dist[port] == kInf) {
        std::println("unreachable");
    } else if (tainted[port] != 0) {
        std::println("unbounded");
    } else {
        std::println("{}", dist[port]);
    }
    return 0;
}
