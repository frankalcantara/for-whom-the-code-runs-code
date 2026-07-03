// cap09/02-shortest-paths.cpp
// Dijkstra, Bellman-Ford, and Floyd-Warshall.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o shortest 02-shortest-paths.cpp

#include <cassert>
#include <algorithm>
#include <climits>
#include <functional>
#include <print>
#include <queue>
#include <vector>

using i64 = long long;
using Edge = std::pair<int,int>;            // (weight, destination)
using WGraph = std::vector<std::vector<Edge>>;

constexpr i64 INF = LLONG_MAX / 2;

// ============================================================
// Dijkstra: O((V + E) log V)
// Requires non-negative edge weights.
// Min-heap of (distance, vertex); relaxes neighbors.
// ============================================================
std::vector<i64> dijkstra(const WGraph& g, int src) {
    int n = static_cast<int>(g.size());
    std::vector<i64> dist(n, INF);
    using State = std::pair<i64, int>;  // (distance, vertex)
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;  // obsolete entry

        for (auto [w, v] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// ============================================================
// Bellman-Ford: O(VE)
// Supports negative edges; detects negative cycles.
// Relaxes every edge V-1 times; if another relaxation improves a distance,
// a reachable negative cycle exists.
// ============================================================
struct BFEdge { int u, v, w; };

struct BFResult { std::vector<i64> dist; bool has_neg_cycle; };

BFResult bellman_ford(int n, const std::vector<BFEdge>& edges, int src) {
    std::vector<i64> dist(n, INF);
    dist[src] = 0;

    for (int iter = 0; iter < n - 1; ++iter)
        for (const auto& [u, v, w] : edges)
            if (dist[u] != INF && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;

    // V-th pass: if relaxation still happens, there is a negative cycle.
    bool neg_cycle = false;
    for (const auto& [u, v, w] : edges)
        if (dist[u] != INF && dist[u] + w < dist[v])
            neg_cycle = true;

    return {dist, neg_cycle};
}

// ============================================================
// Floyd-Warshall: O(V^3) -- all-pairs shortest path
// dp[i][j] = shortest cost from i to j using vertices 0..k as intermediates.
// Negative edges are allowed; a negative cycle appears if dp[i][i] < 0.
// ============================================================
std::vector<std::vector<i64>> floyd_warshall(int n,
    const std::vector<BFEdge>& edges) {

    std::vector<std::vector<i64>> dp(n, std::vector<i64>(n, INF));
    for (int i = 0; i < n; ++i) dp[i][i] = 0;
    for (const auto& [u, v, w] : edges) {
        dp[u][v] = std::min(dp[u][v], (i64)w);
        // For an undirected graph: dp[v][u] = std::min(dp[v][u], (i64)w);
    }

    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dp[i][k] != INF && dp[k][j] != INF)
                    dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k][j]);

    return dp;
}

int main() {
    // Dijkstra: directed weighted graph.
    // 0 -1-> 1, 0 -4-> 2, 1 -2-> 2, 1 -5-> 3, 2 -1-> 3
    WGraph g(4);
    auto aw = [&](int u, int v, int w) { g[u].push_back({w, v}); };
    aw(0,1,1); aw(0,2,4); aw(1,2,2); aw(1,3,5); aw(2,3,1);

    auto dist = dijkstra(g, 0);
    assert(dist[0] == 0);
    assert(dist[1] == 1);
    assert(dist[2] == 3);  // 0->1->2
    assert(dist[3] == 4);  // 0->1->2->3
    std::print("Dijkstra dist[3] = {}\n", dist[3]);

    // Bellman-Ford: same graph.
    std::vector<BFEdge> edges = {{0,1,1},{0,2,4},{1,2,2},{1,3,5},{2,3,1}};
    auto [bf_dist, neg] = bellman_ford(4, edges, 0);
    assert(!neg);
    assert(bf_dist[3] == 4);
    std::print("Bellman-Ford dist[3] = {}\n", bf_dist[3]);

    // Bellman-Ford with a negative cycle.
    std::vector<BFEdge> neg_edges = {{0,1,1},{1,2,-3},{2,0,1}};
    auto [nd, has_neg] = bellman_ford(3, neg_edges, 0);
    assert(has_neg);
    std::print("negative cycle detected: {}\n", has_neg ? "yes" : "no");

    // Floyd-Warshall
    auto fw = floyd_warshall(4, edges);
    assert(fw[0][3] == 4);
    assert(fw[1][3] == 3);  // 1->2->3
    std::print("Floyd-Warshall [0][3]={}, [1][3]={}\n", fw[0][3], fw[1][3]);

    std::print("02-shortest-paths: all assertions passed\n");
    return 0;
}
