// cap09/01-graph-traversal.cpp
// Graph representations, iterative DFS, BFS, components, and topological sort.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o graph_traversal 01-graph-traversal.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fegraph_traversal.exe 01-graph-traversal.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <queue>
#include <stack>
#include <vector>

using Graph = std::vector<std::vector<int>>;  // adjacency list

// ============================================================
// Iterative DFS: visits every vertex reachable from src.
// Returns vertices in visitation order.
// ============================================================
std::vector<int> dfs(const Graph& g, int src) {
    int n = static_cast<int>(g.size());
    std::vector<bool> visited(n, false);
    std::vector<int> order;
    std::stack<int> stk;

    stk.push(src);
    while (!stk.empty()) {
        int u = stk.top(); stk.pop();
        if (visited[u]) continue;
        visited[u] = true;
        order.push_back(u);
        // Pushes neighbors in reverse order to preserve visitation order.
        for (int i = static_cast<int>(g[u].size()) - 1; i >= 0; --i)
            if (!visited[g[u][i]]) stk.push(g[u][i]);
    }
    return order;
}

// ============================================================
// BFS: distances from src to every reachable vertex.
// dist[v] = -1 if unreachable.
// ============================================================
std::vector<int> bfs(const Graph& g, int src) {
    int n = static_cast<int>(g.size());
    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

// ============================================================
// Connected components: counts components with BFS.
// ============================================================
int count_components(const Graph& g) {
    int n = static_cast<int>(g.size());
    std::vector<bool> visited(n, false);
    int count = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++count;
            // BFS of this component.
            std::queue<int> q;
            q.push(i); visited[i] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : g[u])
                    if (!visited[v]) { visited[v] = true; q.push(v); }
            }
        }
    }
    return count;
}

// ============================================================
// Topological sorting: Kahn's algorithm.
// Returns an empty vector if the graph has a cycle.
// in_degree[v] = number of edges entering v.
// ============================================================
std::vector<int> toposort(const Graph& g) {
    int n = static_cast<int>(g.size());
    std::vector<int> in_degree(n, 0);
    for (int u = 0; u < n; ++u)
        for (int v : g[u]) ++in_degree[v];

    std::queue<int> q;
    for (int u = 0; u < n; ++u)
        if (in_degree[u] == 0) q.push(u);

    std::vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u])
            if (--in_degree[v] == 0) q.push(v);
    }
    return (static_cast<int>(order.size()) == n) ? order : std::vector<int>{};
}

int main() {
    // Undirected graph: 5 vertices, 4 edges.
    Graph g(5);
    auto add_edge = [&](int u, int v) { g[u].push_back(v); g[v].push_back(u); };
    add_edge(0, 1); add_edge(0, 2); add_edge(1, 3); add_edge(3, 4);

    auto dfs_order = dfs(g, 0);
    assert(static_cast<int>(dfs_order.size()) == 5);
    std::print("DFS from 0: ");
    for (int v : dfs_order) std::print("{} ", v);
    std::print("\n");

    auto dist = bfs(g, 0);
    assert(dist[4] == 3);  // 0->1->3->4
    assert(dist[2] == 1);
    std::print("BFS dist[4] = {}\n", dist[4]);

    // Components: includes isolated vertices.
    Graph g2(6);
    g2[0].push_back(1); g2[1].push_back(0);
    g2[2].push_back(3); g2[3].push_back(2);
    // vertices 4 and 5 are isolated
    assert(count_components(g2) == 4);
    std::print("components: {}\n", count_components(g2));

    // Toposort: DAG 0->1->3, 0->2->3, 3->4
    Graph dag(5);
    dag[0].push_back(1); dag[0].push_back(2);
    dag[1].push_back(3); dag[2].push_back(3);
    dag[3].push_back(4);
    auto topo = toposort(dag);
    assert(!topo.empty());
    // Verifies that for each edge (u,v), u appears before v.
    std::vector<int> pos(5);
    for (int i = 0; i < 5; ++i) pos[topo[i]] = i;
    assert(pos[0] < pos[1] && pos[0] < pos[2]);
    assert(pos[1] < pos[3] && pos[2] < pos[3]);
    assert(pos[3] < pos[4]);
    std::print("toposort: ");
    for (int v : topo) std::print("{} ", v);
    std::print("\n");

    // Graph with a cycle: toposort returns empty.
    Graph cyclic(3);
    cyclic[0].push_back(1); cyclic[1].push_back(2); cyclic[2].push_back(0);
    assert(toposort(cyclic).empty());
    std::print("toposort with cycle: empty (correct)\n");

    std::print("01-graph-traversal: all assertions passed\n");
    return 0;
}
