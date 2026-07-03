// Exercise T09.1 - The Postman's Districts
// Session: T09.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 09 (adjacency list, BFS, connected components)
// Tags: chapter-09, bfs
//
// MODEL: connected components of an undirected graph. Each component
//   is one round: reachability is an equivalence relation (reflexive,
//   symmetric on undirected edges, transitive by concatenating walks),
//   and the components are its classes.
// MATH: repeatedly pick an unvisited hamlet, BFS from it marking
//   everything reachable, count one component per start. Each vertex
//   is enqueued at most once and each edge inspected at most twice
//   (once per endpoint).
// PROOF: BFS from s visits exactly the vertices connected to s
//   (induction on BFS layers for "only connected"; induction on path
//   length for "all connected"). The outer scan starts a BFS only on
//   unvisited vertices, so each equivalence class is counted exactly
//   once - the first time the scan touches it.
// COMPLEXITY: O(n + m) time and memory with adjacency lists. An
//   adjacency MATRIX is 4e10 bits at n = 2e5 - the representation
//   chapter's first lesson is that this choice is the algorithm.
// TYPES: vertex ids fit int; component counts fit int. The adjacency
//   list stores 2m ints (~1.6 MB): nothing here strains anything,
//   which is the right difficulty for a warm-up whose only trap is
//   forgetting that isolated hamlets (degree 0) are components too.
// ALTERNATIVES: DFS does the same in the same bounds (recursion depth
//   2e5 risks the default stack - iterative or BFS is safer in a
//   contest); DSU (Chapter 6, the Rope Bridges road) answers the same
//   count without ever materializing the graph, the better tool when
//   edges arrive as a stream. BFS is chosen here because this unit is
//   about walking graphs, not merging them.
// EDGE CASES: m = 0 (n components); a single component spanning all
//   hamlets (1); self-loops a == b (allowed by the contract's
//   silence, harmless: the vertex is already visited); parallel paths
//   (revisits are skipped by the mark).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o districts t09-s1-1-postmans-districts.cpp

#include <iostream>
#include <print>
#include <queue>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(static_cast<std::size_t>(n) + 1);
    for (int e = 0; e < m; ++e) {
        int a = 0;
        int b = 0;
        std::cin >> a >> b;
        adj[static_cast<std::size_t>(a)].push_back(b);
        adj[static_cast<std::size_t>(b)].push_back(a);
    }

    std::vector<bool> visited(static_cast<std::size_t>(n) + 1, false);
    int rounds = 0;
    std::queue<int> frontier;
    for (int s = 1; s <= n; ++s) {
        if (visited[static_cast<std::size_t>(s)]) continue;
        ++rounds;
        visited[static_cast<std::size_t>(s)] = true;
        frontier.push(s);
        while (!frontier.empty()) {
            const int u = frontier.front();
            frontier.pop();
            for (const int v : adj[static_cast<std::size_t>(u)]) {
                if (!visited[static_cast<std::size_t>(v)]) {
                    visited[static_cast<std::size_t>(v)] = true;
                    frontier.push(v);
                }
            }
        }
    }
    std::print("{}\n", rounds);
    return 0;
}
