// cap06/07-dsu.cpp
// Union-Find (Disjoint Set Union) -- path compression + union by rank.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o dsu 07-dsu.cpp

#include <cassert>
#include <algorithm>
#include <numeric>
#include <print>
#include <vector>

// ============================================================
// Classic DSU: almost O(1) amortized per operation (inverse Ackermann).
// Path compression: find() flattens the tree on each call.
// Union by rank: attaches the smaller tree under the larger one.
// Together: amortized O(alpha(n)), effectively constant in practice.
// ============================================================
struct DSU {
    std::vector<int> parent, rank_;
    int components;

    explicit DSU(int n)
        : parent(n), rank_(n, 0), components(n)
    {
        std::iota(parent.begin(), parent.end(), 0);
    }

    // Finds the root with path compression: O(alpha(n)).
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    // Merges the sets of x and y; returns true if they were distinct.
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        --components;
        if (rank_[x] < rank_[y]) std::swap(x, y);
        parent[y] = x;
        if (rank_[x] == rank_[y]) ++rank_[x];
        return true;
    }

    bool same(int x, int y) { return find(x) == find(y); }
};

// ============================================================
// Weighted DSU.
// Stores the weight difference between each node and its root.
// Used for parity, relative-distance, and consistency problems.
// ============================================================
struct WeightedDSU {
    std::vector<int> parent, rank_;
    std::vector<long long> diff;  // diff[x] = weight(x) - weight(find(x))

    explicit WeightedDSU(int n)
        : parent(n), rank_(n, 0), diff(n, 0)
    {
        std::iota(parent.begin(), parent.end(), 0);
    }

    // Returns {root, accumulated difference from x to the root}.
    std::pair<int, long long> find(int x) {
        if (parent[x] == x) return {x, 0};
        auto [root, d] = find(parent[x]);
        parent[x] = root;
        diff[x] += d;
        return {root, diff[x]};
    }

    // Merges x and y with relation: weight(y) - weight(x) = w.
    bool unite(int x, int y, long long w) {
        auto [rx, dx] = find(x);
        auto [ry, dy] = find(y);
        if (rx == ry) return false;
        w = w + dx - dy;
        if (rank_[rx] < rank_[ry]) { std::swap(rx, ry); w = -w; }
        parent[ry] = rx;
        diff[ry] = w;
        if (rank_[rx] == rank_[ry]) ++rank_[rx];
        return true;
    }

    // Weight difference between x and y (requires same(x,y)).
    long long diff_between(int x, int y) {
        auto [_, dx] = find(x);
        auto [__, dy] = find(y);
        return dy - dx;  // weight(y) - weight(x)
    }
};

// ============================================================
// Classic problem: connected components and Kruskal MST.
// ============================================================
// Kruskal: sorts edges by weight and merges with DSU -- O(m log m).
struct Edge { int u, v, w; };

long long kruskal_mst(int n, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b){ return a.w < b.w; });
    DSU dsu(n);
    long long total = 0;
    for (const auto& [u, v, w] : edges)
        if (dsu.unite(u, v)) total += w;
    return total;
}

int main() {
    // Basic DSU
    DSU dsu(6);
    assert(dsu.components == 6);
    dsu.unite(0, 1); dsu.unite(2, 3); dsu.unite(4, 5);
    assert(dsu.components == 3);
    dsu.unite(0, 2);
    assert(dsu.components == 2);
    assert(dsu.same(0, 3));
    assert(!dsu.same(0, 4));
    std::print("DSU: {} components\n", dsu.components);

    // Weighted DSU
    WeightedDSU wdsu(4);
    wdsu.unite(0, 1, 3);  // weight(1) - weight(0) = 3
    wdsu.unite(1, 2, 5);  // weight(2) - weight(1) = 5
    // Therefore weight(2) - weight(0) = 8
    assert(wdsu.diff_between(0, 2) == 8);
    std::print("WeightedDSU: diff(0,2) = {}\n", wdsu.diff_between(0, 2));

    // Kruskal MST
    // Graph: 0-1 (1), 0-2 (3), 1-2 (2), 1-3 (4), 2-3 (5)
    // MST: 0-1 (1) + 1-2 (2) + 1-3 (4) = 7
    std::vector<Edge> edges = {{0,1,1},{0,2,3},{1,2,2},{1,3,4},{2,3,5}};
    long long mst = kruskal_mst(4, edges);
    assert(mst == 7);
    std::print("Kruskal MST: {}\n", mst);

    std::print("07-dsu: all assertions passed\n");
    return 0;
}
