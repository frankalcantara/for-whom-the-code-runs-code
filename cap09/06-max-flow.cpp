// cap09/06-max-flow.cpp
// Maximum flow with Dinic's algorithm.
// Also demonstrates bipartite matching as a max-flow reduction.
//
// Dinic's algorithm complexity: O(V^2 * E)
// For unit-capacity graphs (e.g., bipartite matching): O(E * sqrt(V))
//
// Compile: g++ -std=c++23 -O2 -o max-flow 06-max-flow.cpp

#include <algorithm>
#include <climits>
#include <print>
#include <queue>
#include <utility>
#include <vector>

// ===== Dinic's Max Flow =====
//
// The key data structure is the adjacency list with explicit reverse edges.
// Each edge stores: destination, index of reverse edge, remaining capacity.
// When we push f units along edge i, we subtract f from edge i's capacity
// and add f to the reverse edge's capacity (making the flow cancellable).

struct MaxFlow {
    struct Edge {
        int to;   // destination vertex
        int rev;  // index of reverse edge in g[to]
        int cap;  // remaining capacity
    };

    int n;
    std::vector<std::vector<Edge>> g;
    std::vector<int> level;  // BFS level (distance from source)
    std::vector<int> iter;   // current edge pointer for DFS (dead-end pruning)

    explicit MaxFlow(int n) : n(n), g(n), level(n), iter(n) {}

    // Add directed edge u→v with capacity cap.
    // The reverse edge v→u is added with capacity 0.
    void add_edge(int u, int v, int cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    // BFS from source s: build layered graph.
    // Returns true if sink t is reachable (more flow is possible).
    bool bfs(int s, int t) {
        std::ranges::fill(level, -1);
        std::queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : g[u]) {
                // Only traverse edges with remaining capacity
                // and vertices not yet assigned a level.
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    // DFS: push as much flow as possible along admissible edges.
    // An edge u→v is admissible when level[v] == level[u] + 1.
    // iter[u] remembers where we left off so dead ends are not revisited.
    int dfs(int u, int t, int pushed) {
        if (u == t) return pushed;
        for (int& i = iter[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];
            if (e.cap > 0 && level[u] < level[e.to]) {
                int d = dfs(e.to, t, std::min(pushed, e.cap));
                if (d > 0) {
                    e.cap -= d;                // consume forward capacity
                    g[e.to][e.rev].cap += d;   // restore reverse capacity
                    return d;
                }
            }
        }
        return 0;
    }

    // Run Dinic: BFS to build layers, DFS to push blocking flow, repeat.
    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            std::ranges::fill(iter, 0);
            int f;
            while ((f = dfs(s, t, INT_MAX)) > 0)
                flow += f;
        }
        return flow;
    }
};

// ===== Bipartite Matching via Max Flow =====
//
// Left vertices: 0..L-1
// Right vertices: L..L+R-1
// Super source: L+R
// Super sink:   L+R+1
//
// Add edges: source → each left (cap 1)
//            each right → sink (cap 1)
//            each (u, v) pair → left_u → right_v (cap 1)
//
// Max flow = size of maximum bipartite matching.

int bipartite_matching(
    int L,
    int R,
    const std::vector<std::pair<int, int>>& edges
) {
    int s = L + R, t = L + R + 1;
    MaxFlow mf(t + 1);

    for (int u = 0; u < L; ++u)
        mf.add_edge(s, u, 1);

    for (int v = 0; v < R; ++v)
        mf.add_edge(L + v, t, 1);

    for (auto [u, v] : edges)
        mf.add_edge(u, L + v, 1);

    return mf.max_flow(s, t);
}

// ===== Demo =====

int main() {
    // --- Max flow demo ---
    // Graph: s=0, a=1, b=2, c=3, t=4
    //  s→a cap 3, s→b cap 2
    //  a→c cap 3, b→c cap 2, b→t cap 2
    //  c→t cap 4
    MaxFlow mf(5);
    mf.add_edge(0, 1, 3);  // s→a
    mf.add_edge(0, 2, 2);  // s→b
    mf.add_edge(1, 3, 3);  // a→c
    mf.add_edge(2, 3, 2);  // b→c
    mf.add_edge(2, 4, 2);  // b→t
    mf.add_edge(3, 4, 4);  // c→t

    // Max flow: min-cut limits to 5 (s→a cap 3, b→t cap 2)
    std::print("Max flow s->t: {}\n", mf.max_flow(0, 4));  // 5

    // --- Bipartite matching demo ---
    // L=3 tasks, R=3 workers
    // task 0 can be done by workers 0, 1
    // task 1 can be done by workers 1, 2
    // task 2 can be done by worker 2
    std::vector<std::pair<int, int>> edges = {{0,0},{0,1},{1,1},{1,2},{2,2}};
    std::print("Max matching: {}\n", bipartite_matching(3, 3, edges));  // 3

    return 0;
}
