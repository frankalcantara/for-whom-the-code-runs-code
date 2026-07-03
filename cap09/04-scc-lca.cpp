// cap09/04-scc-lca.cpp
// Strongly Connected Components (Kosaraju) and LCA with binary lifting.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o scc_lca 04-scc-lca.cpp

#include <algorithm>
#include <cassert>
#include <climits>
#include <queue>
#include <ranges>
#include <string>
#include <print>
#include <vector>

using Graph = std::vector<std::vector<int>>;

// ============================================================
// Kosaraju: two DFS passes -- O(V + E).
// 1) DFS on the original graph records vertices by finish time.
// 2) DFS on the transposed graph labels one SCC per traversal.
// ============================================================
void dfs_finish(const Graph& g, int u, std::vector<bool>& vis,
                std::vector<int>& order) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs_finish(g, v, vis, order);
    order.push_back(u);
}

void dfs_assign(const Graph& g, int u, int comp, std::vector<int>& scc) {
    scc[u] = comp;
    for (int v : g[u]) if (scc[v] == -1) dfs_assign(g, v, comp, scc);
}

// Returns scc[v] = id of v's strongly connected component (0-indexed).
std::vector<int> kosaraju(const Graph& g) {
    int n = static_cast<int>(g.size());
    std::vector<bool> vis(n, false);
    std::vector<int> order;

    for (int u = 0; u < n; ++u)
        if (!vis[u]) dfs_finish(g, u, vis, order);

    // Transposed graph.
    Graph gt(n);
    for (int u = 0; u < n; ++u)
        for (int v : g[u]) gt[v].push_back(u);

    std::vector<int> scc(n, -1);
    int comp = 0;
    for (int i = n - 1; i >= 0; --i)
        if (scc[order[i]] == -1)
            dfs_assign(gt, order[i], comp++, scc);

    return scc;
}

// ============================================================
// LCA with binary lifting: O(n log n) preprocessing, O(log n) query.
// Given a rooted tree and two nodes u and v, find their lowest common ancestor.
// up[k][v] = 2^k-th ancestor of v.
// ============================================================
struct LCA {
    int n, LOG;
    std::vector<int> depth;
    std::vector<std::vector<int>> up;

    LCA(const Graph& g, int root)
        : n(static_cast<int>(g.size()))
        , LOG(1)
        , depth(g.size(), 0)
    {
        while ((1 << LOG) < n) ++LOG;
        up.assign(LOG, std::vector<int>(n, root));

        // BFS initializes depth and up[0].
        std::vector<bool> vis(n, false);
        std::queue<int> q;
        q.push(root); vis[root] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (!vis[v]) {
                    vis[v] = true;
                    depth[v] = depth[u] + 1;
                    up[0][v] = u;
                    q.push(v);
                }
            }
        }
        up[0][root] = root;

        for (int k = 1; k < LOG; ++k)
            for (int v = 0; v < n; ++v)
                up[k][v] = up[k-1][up[k-1][v]];
    }

    // Lifts v by k steps.
    int lift(int v, int k) const {
        for (int i = 0; i < LOG; ++i)
            if (k >> i & 1) v = up[i][v];
        return v;
    }

    int query(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);
        u = lift(u, depth[u] - depth[v]);  // equalizes depths
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; --k)
            if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }
        return up[0][u];
    }
};


int main() {
    // SCC: classic graph with 8 vertices.
    // SCCs: {0,1,2,3}, {4}, {5,6}, {7}
    Graph g(8);
    auto ae = [&](int u, int v) { g[u].push_back(v); };
    ae(0,1); ae(1,2); ae(2,0); ae(2,3); ae(3,4);
    ae(4,5); ae(5,6); ae(6,4); ae(6,7);

    auto scc = kosaraju(g);
    // Verifies selected component relations.
    assert(scc[0] == scc[1] && scc[1] == scc[2]);
    assert(scc[5] == scc[6]);
    assert(scc[3] != scc[0]);  // 3 does not belong to SCC {0,1,2}
    assert(scc[4] == scc[5] && scc[5] == scc[6]);  // {4,5,6} is an SCC
    assert(scc[7] != scc[6]);  // 7 is outside SCC {4,5,6}
    // Counts distinct components.
    int num_scc = std::ranges::max(scc) + 1;
    assert(num_scc == 4);
    std::print("SCCs: {} components\n", num_scc);

    // LCA: tree with 7 vertices.
    //         0
    //       /   \
    //      1     2
    //     / \   / \
    //    3   4 5   6
    Graph tree(7);
    auto at = [&](int u, int v) { tree[u].push_back(v); tree[v].push_back(u); };
    at(0,1); at(0,2); at(1,3); at(1,4); at(2,5); at(2,6);

    LCA lca(tree, 0);
    assert(lca.query(3, 4) == 1);   // LCA(3,4) = 1
    assert(lca.query(3, 5) == 0);   // LCA(3,5) = 0
    assert(lca.query(5, 6) == 2);   // LCA(5,6) = 2
    assert(lca.query(0, 6) == 0);   // LCA(0,6) = 0
    assert(lca.query(3, 3) == 3);   // LCA(v,v) = v
    std::print("LCA(3,4)={}, LCA(3,5)={}, LCA(5,6)={}\n",
        lca.query(3,4), lca.query(3,5), lca.query(5,6));

    std::print("04-scc-lca: all assertions passed\n");
    return 0;
}
