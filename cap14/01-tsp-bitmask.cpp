// cap14/01-tsp-bitmask.cpp
// Exact exponential algorithms for small instances: TSP, vertex cover, and k-coloring.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o tsp 01-tsp-bitmask.cpp

#include <algorithm>
#include <cassert>
#include <bit>
#include <climits>
#include <functional>
#include <print>
#include <vector>

using i64 = long long;
constexpr i64 INF = LLONG_MAX / 4;

i64 tsp(const std::vector<std::vector<int>>& dist) {
    int n = static_cast<int>(dist.size());
    if (n == 0) return 0;
    unsigned full = (1u << n) - 1u;

    std::vector<std::vector<i64>> dp(1u << n, std::vector<i64>(n, INF));
    dp[1][0] = 0;

    for (unsigned mask = 1; mask <= full; ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1u << u)) || dp[mask][u] == INF) continue;
            for (int v = 0; v < n; ++v) {
                if (mask & (1u << v)) continue;
                if (dist[u][v] < 0) continue;
                unsigned new_mask = mask | (1u << v);
                dp[new_mask][v] = std::min(dp[new_mask][v],
                                           dp[mask][u] + dist[u][v]);
            }
        }
    }

    i64 ans = INF;
    for (int u = 1; u < n; ++u)
        if (dp[full][u] != INF && dist[u][0] >= 0)
            ans = std::min(ans, dp[full][u] + dist[u][0]);
    return ans;
}

int min_vertex_cover(int n, const std::vector<std::pair<int, int>>& edges) {
    for (unsigned mask = 0; mask < (1u << n); ++mask) {
        bool covers = true;
        for (auto [u, v] : edges) {
            if (!(mask & (1u << u)) && !(mask & (1u << v))) {
                covers = false;
                break;
            }
        }
        if (covers) return static_cast<int>(std::popcount(mask));
    }
    return n;
}

bool is_k_colorable(int n, const std::vector<std::pair<int, int>>& edges, int k) {
    std::vector<std::vector<int>> adj(n);
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> order(n), color(n, -1);
    for (int i = 0; i < n; ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return adj[a].size() > adj[b].size();
    });

    std::function<bool(int)> bt = [&](int pos) -> bool {
        if (pos == n) return true;
        int u = order[pos];
        for (int c = 0; c < k; ++c) {
            bool ok = true;
            for (int v : adj[u]) {
                if (color[v] == c) { ok = false; break; }
            }
            if (!ok) continue;
            color[u] = c;
            if (bt(pos + 1)) return true;
            color[u] = -1;
        }
        return false;
    };

    return bt(0);
}

int main() {
    std::vector<std::vector<int>> d4 = {
        {-1, 10, 15, 10},
        {10, -1, 10, 15},
        {15, 10, -1, 10},
        {10, 15, 10, -1}
    };
    assert(tsp(d4) == 40);

    std::vector<std::vector<int>> d3 = {
        {-1, 5, 3},
        {5, -1, 2},
        {3, 2, -1}
    };
    assert(tsp(d3) == 10);

    assert(min_vertex_cover(4, {{0, 1}, {1, 2}, {2, 3}}) == 2);
    assert(min_vertex_cover(4, {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}) == 3);

    assert(is_k_colorable(4, {{0, 2}, {0, 3}, {1, 2}, {1, 3}}, 2));
    assert(!is_k_colorable(4, {{0, 2}, {0, 3}, {1, 2}, {1, 3}}, 1));
    assert(is_k_colorable(3, {{0, 1}, {1, 2}, {0, 2}}, 3));
    assert(!is_k_colorable(3, {{0, 1}, {1, 2}, {0, 2}}, 2));

    std::print("01-tsp-bitmask: all assertions passed\n");
    return 0;
}
