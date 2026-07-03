// cap14/02-approximation.cpp
// Approximation algorithms and branch-and-bound for hard optimization problems.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o approx 02-approximation.cpp

#include <algorithm>
#include <cassert>
#include <climits>
#include <functional>
#include <numeric>
#include <print>
#include <tuple>
#include <vector>

using i64 = long long;

std::vector<int> approx_vertex_cover(int n,
                                     const std::vector<std::pair<int, int>>& edges) {
    std::vector<bool> in_cover(n, false);
    std::vector<int> cover;
    for (auto [u, v] : edges) {
        if (!in_cover[u] && !in_cover[v]) {
            in_cover[u] = in_cover[v] = true;
            cover.push_back(u);
            cover.push_back(v);
        }
    }
    return cover;
}

struct DSU {
    std::vector<int> p;
    explicit DSU(int n) : p(n) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        p[y] = x;
        return true;
    }
};

i64 approx_tsp_metric(int n, const std::vector<std::vector<int>>& dist) {
    using Edge = std::tuple<int, int, int>;
    std::vector<Edge> edges;
    for (int u = 0; u < n; ++u)
        for (int v = u + 1; v < n; ++v)
            if (dist[u][v] >= 0) edges.push_back({dist[u][v], u, v});
    std::sort(edges.begin(), edges.end());

    DSU dsu(n);
    std::vector<std::vector<int>> mst(n);
    for (auto [w, u, v] : edges) {
        (void)w;
        if (dsu.unite(u, v)) {
            mst[u].push_back(v);
            mst[v].push_back(u);
        }
    }

    std::vector<int> tour;
    std::vector<bool> vis(n, false);
    std::function<void(int)> dfs = [&](int u) {
        vis[u] = true;
        tour.push_back(u);
        for (int v : mst[u]) if (!vis[v]) dfs(v);
    };
    dfs(0);

    i64 cost = 0;
    for (int i = 0; i < n; ++i)
        cost += dist[tour[i]][tour[(i + 1) % n]];
    return cost;
}

struct TspBranchBound {
    int n;
    const std::vector<std::vector<int>>& dist;
    i64 best = LLONG_MAX / 4;
    std::vector<bool> visited;
    i64 current_cost = 0;

    explicit TspBranchBound(const std::vector<std::vector<int>>& d)
        : n(static_cast<int>(d.size())), dist(d), visited(n, false) {}

    i64 lower_bound() const {
        i64 lb = current_cost;
        for (int u = 0; u < n; ++u) {
            if (visited[u]) continue;
            int min_out = INT_MAX;
            for (int v = 0; v < n; ++v)
                if (u != v && dist[u][v] >= 0)
                    min_out = std::min(min_out, dist[u][v]);
            if (min_out == INT_MAX) return best;
            lb += min_out;
        }
        return lb;
    }

    void solve(int depth, int last) {
        if (depth == n) {
            if (dist[last][0] >= 0)
                best = std::min(best, current_cost + dist[last][0]);
            return;
        }
        if (lower_bound() >= best) return;

        for (int v = 0; v < n; ++v) {
            if (visited[v] || dist[last][v] < 0) continue;
            visited[v] = true;
            current_cost += dist[last][v];
            solve(depth + 1, v);
            current_cost -= dist[last][v];
            visited[v] = false;
        }
    }

    i64 run() {
        visited[0] = true;
        solve(1, 0);
        return best;
    }
};

int main() {
    auto vc = approx_vertex_cover(4, {{0, 1}, {1, 2}, {2, 3}});
    for (auto [u, v] : std::vector<std::pair<int, int>>{{0, 1}, {1, 2}, {2, 3}}) {
        bool covered = false;
        for (int x : vc) if (x == u || x == v) { covered = true; break; }
        assert(covered);
    }
    assert(static_cast<int>(vc.size()) <= 4);

    std::vector<std::vector<int>> d4 = {
        {-1, 10, 15, 10},
        {10, -1, 10, 15},
        {15, 10, -1, 10},
        {10, 15, 10, -1}
    };
    i64 approx = approx_tsp_metric(4, d4);
    assert(approx >= 40 && approx <= 80);

    TspBranchBound bb(d4);
    assert(bb.run() == 40);

    std::vector<std::vector<int>> d3 = {{-1, 5, 3}, {5, -1, 2}, {3, 2, -1}};
    TspBranchBound bb3(d3);
    assert(bb3.run() == 10);

    std::print("02-approximation: all assertions passed\n");
    return 0;
}
