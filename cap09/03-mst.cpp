// cap09/03-mst.cpp
// Minimum Spanning Tree: Kruskal (DSU) and Prim (heap).
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o mst 03-mst.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femst.exe 03-mst.cpp

#include <cassert>
#include <algorithm>
#include <functional>
#include <numeric>
#include <tuple>
#include <print>
#include <queue>
#include <vector>

using i64 = long long;
using Edge = std::tuple<int,int,int>;  // (weight, u, v)
using WGraph = std::vector<std::vector<std::pair<int,int>>>;  // (weight, destination)

// ============================================================
// DSU for Kruskal.
// ============================================================
struct DSU {
    std::vector<int> p, rank_;
    DSU(int n) : p(n), rank_(n, 0) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rank_[x] < rank_[y]) std::swap(x, y);
        p[y] = x;
        if (rank_[x] == rank_[y]) ++rank_[x];
        return true;
    }
};

// ============================================================
// Kruskal: O(E log E) because of sorting.
// Sorts edges by weight and includes edges that do not create a cycle.
// ============================================================
i64 kruskal(int n, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end());
    DSU dsu(n);
    i64 total = 0;
    int count = 0;
    for (const auto& [w, u, v] : edges) {
        if (dsu.unite(u, v)) {
            total += w;
            if (++count == n - 1) break;  // MST complete
        }
    }
    return total;
}

// ============================================================
// Prim: O((V + E) log V) with a min-heap.
// Starts from one vertex and repeatedly adds the cheapest frontier edge.
// ============================================================
i64 prim(const WGraph& g, int src) {
    int n = static_cast<int>(g.size());
    std::vector<bool> in_mst(n, false);
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;

    pq.push({0, src});
    i64 total = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true;
        total += w;

        for (auto [wv, v] : g[u])
            if (!in_mst[v]) pq.push({wv, v});
    }
    return total;
}

int main() {
    // Undirected graph: 4 vertices.
    // 0-1: 10, 0-2: 6, 0-3: 5, 1-3: 15, 2-3: 4
    // MST: 2-3 (4) + 0-3 (5) + 0-1 (10) = 19
    std::vector<Edge> edges = {
        {10,0,1},{6,0,2},{5,0,3},{15,1,3},{4,2,3}
    };
    i64 mst_k = kruskal(4, edges);
    assert(mst_k == 19);
    std::print("Kruskal MST = {}\n", mst_k);

    // Same graph for Prim.
    WGraph g(4);
    auto ae = [&](int u, int v, int w) {
        g[u].push_back({w, v}); g[v].push_back({w, u});
    };
    ae(0,1,10); ae(0,2,6); ae(0,3,5); ae(1,3,15); ae(2,3,4);
    i64 mst_p = prim(g, 0);
    assert(mst_p == 19);
    std::print("Prim MST    = {}\n", mst_p);
    assert(mst_k == mst_p);

    // Larger graph: 6 vertices.
    WGraph g2(6);
    auto ae2 = [&](int u, int v, int w) {
        g2[u].push_back({w,v}); g2[v].push_back({w,u});
    };
    ae2(0,1,4); ae2(0,2,3); ae2(1,2,1); ae2(1,3,2);
    ae2(2,3,4); ae2(3,4,2); ae2(4,5,6); ae2(3,5,5);

    std::vector<Edge> e2 = {
        {4,0,1},{3,0,2},{1,1,2},{2,1,3},{4,2,3},{2,3,4},{6,4,5},{5,3,5}
    };
    i64 mk2 = kruskal(6, e2);
    i64 mp2 = prim(g2, 0);
    assert(mk2 == mp2);
    std::print("MST 6-vertices: Kruskal={}, Prim={}\n", mk2, mp2);

    std::print("03-mst: all assertions passed\n");
    return 0;
}
