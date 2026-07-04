// Exercise T09.2 - The Stagecoach Lines
// Session: T09.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 09 (Dijkstra with a lazy-deletion priority queue)
// Tags: chapter-09, greedy, dijkstra
//
// MODEL: single-source shortest path with nonnegative edge weights -
//   Dijkstra's home ground. Fares are positive, so once a town's
//   cheapest cost is settled it can never improve, which is the whole
//   theorem.
// MATH: maintain dist[], initially infinite except dist[1] = 0, and a
//   min-heap of (cost, town) candidates. Pop the cheapest candidate;
//   if stale (cost > dist[town]), discard - lazy deletion; otherwise
//   settle the town and relax its edges, pushing improved candidates.
//   Each edge pushes at most one candidate, so the heap holds O(m)
//   entries.
// PROOF: greedy invariant, by induction on settlements: when (c, u)
//   is popped fresh, c = dist[u] is optimal - any other route to u
//   would leave the settled region through some frontier candidate
//   with cost <= its total (weights are nonnegative from there on),
//   and the heap popped the minimum, so no such route beats c. Lazy
//   deletion is sound because a stale entry's town was settled at a
//   value <= the entry, and the fresh-check discards it untouched.
//   Negative fares would break exactly this "leaving cannot help"
//   step - that is Bellman-Ford's jurisdiction (and not this map's).
// COMPLEXITY: O((n + m) log m) time with the binary heap (~4e6 heap
//   operations at the limits), O(n + m) memory. Floyd-Warshall is
//   O(n^3): 8e15, a number to laugh at and move on.
// TYPES: a route may chain 2e5 fares of 1e9: totals reach 2e14, so
//   dist and heap costs are long long. The sentinel "infinity" must
//   sit above any real total but below overflow when an edge is
//   added: numeric_limits<long long>::max() / 4 does both jobs.
// ALTERNATIVES: a set<pair> with true deletions matches the bound
//   with larger constants; 0-1 BFS needs weights in {0, 1} (T09.5's
//   business); bidirectional Dijkstra halves the searched ball for
//   single-pair queries - profitable at scale, not at 2e5.
// EDGE CASES: n = 1 (cost 0, no travel); town n unreachable;
//   parallel lines between the same towns (relaxation keeps the
//   cheapest); a tempting direct line beaten by a chain (the
//   example); maximum-length cheapest route (overflow drill).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o coach t09-s2-1-stagecoach-lines.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecoach.exe t09-s2-1-stagecoach-lines.cpp

#include <iostream>
#include <limits>
#include <print>
#include <queue>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<std::pair<int, long long>>> adj(static_cast<std::size_t>(n) + 1);
    for (int e = 0; e < m; ++e) {
        int a = 0;
        int b = 0;
        long long f = 0;
        std::cin >> a >> b >> f;
        adj[static_cast<std::size_t>(a)].emplace_back(b, f);
        adj[static_cast<std::size_t>(b)].emplace_back(a, f);
    }

    constexpr long long kInfinity = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> dist(static_cast<std::size_t>(n) + 1, kInfinity);
    using Candidate = std::pair<long long, int>;  // (cost, town)
    std::priority_queue<Candidate, std::vector<Candidate>, std::greater<>> heap;

    dist[1] = 0;
    heap.emplace(0, 1);
    while (!heap.empty()) {
        const auto [cost, u] = heap.top();
        heap.pop();
        if (cost > dist[static_cast<std::size_t>(u)]) continue;  // stale
        if (u == n) break;  // destination settled; the rest is optional work
        for (const auto& [v, fare] : adj[static_cast<std::size_t>(u)]) {
            if (cost + fare < dist[static_cast<std::size_t>(v)]) {
                dist[static_cast<std::size_t>(v)] = cost + fare;
                heap.emplace(dist[static_cast<std::size_t>(v)], v);
            }
        }
    }

    if (dist[static_cast<std::size_t>(n)] >= kInfinity) {
        std::print("unreachable\n");
    } else {
        std::print("{}\n", dist[static_cast<std::size_t>(n)]);
    }
    return 0;
}
