// Exercise T09.4 - The Wedding Tables
// Session: T09.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 09 (bipartiteness check by BFS 2-coloring)
// Tags: chapter-09, bfs
//
// MODEL: the feud graph must be bipartite: a valid seating is exactly
//   a proper 2-coloring (color = table). The structural criterion is
//   Konig's classic: a graph is bipartite iff it has no odd cycle -
//   the example's pentagon is the smallest wedding disaster.
// MATH: BFS each component from an arbitrary guest, assigning tables
//   by parity of BFS distance: neighbors get the opposite table. If
//   an edge ever joins two guests of the same assigned table, report
//   impossible.
// PROOF: if the BFS finishes with no conflict, the assignment is a
//   proper 2-coloring by construction (every edge was checked when
//   relaxed from its first-discovered endpoint... and from the other
//   side when scanned there - undirected adjacency lists scan each
//   edge twice, so no edge escapes). If a conflict edge (u, v)
//   appears, dist[u] and dist[v] have equal parity; the BFS-tree
//   paths from the root plus the edge (u, v) contain an odd closed
//   walk, and every odd closed walk contains an odd cycle, which no
//   2-coloring colors properly - so impossible is certified, not
//   guessed. Each component is independent: colors never interact
//   across components.
// COMPLEXITY: O(n + m) time and memory. There is nothing faster to
//   wish for: every feud must at least be read.
// TYPES: table labels live in an int8-ish vector<signed char> (-1
//   unassigned, 0, 1) - 2e5 entries; parity arithmetic via XOR with
//   1. Nothing overflows; the exercise's risk is forgetting that the
//   feud graph may be disconnected and BFS-ing only guest 1's circle.
// ALTERNATIVES: DSU with parity (each guest unioned to its foe's
//   "opposite" class) answers the same question online and is the
//   tool when feuds arrive as a stream; DFS coloring matches BFS
//   offline but recurses deep on path-like feud chains. The chapter's
//   BFS habit wins on safety.
// EDGE CASES: m = 0 (trivially possible); a guest feuding with
//   themselves (a == b: impossible by definition, the check catches
//   it as a same-color edge); even cycles (possible); two odd cycles
//   in different components; duplicate feud pairs (idempotent).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tables t09-s3-2-wedding-tables.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetables.exe t09-s3-2-wedding-tables.cpp

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
    bool self_feud = false;
    for (int e = 0; e < m; ++e) {
        int a = 0;
        int b = 0;
        std::cin >> a >> b;
        if (a == b) self_feud = true;
        adj[static_cast<std::size_t>(a)].push_back(b);
        adj[static_cast<std::size_t>(b)].push_back(a);
    }

    std::vector<signed char> table(static_cast<std::size_t>(n) + 1, -1);
    bool ok = !self_feud;
    std::queue<int> frontier;
    for (int s = 1; s <= n && ok; ++s) {
        if (table[static_cast<std::size_t>(s)] != -1) continue;
        table[static_cast<std::size_t>(s)] = 0;
        frontier.push(s);
        while (!frontier.empty() && ok) {
            const int u = frontier.front();
            frontier.pop();
            for (const int v : adj[static_cast<std::size_t>(u)]) {
                if (table[static_cast<std::size_t>(v)] == -1) {
                    table[static_cast<std::size_t>(v)] =
                        static_cast<signed char>(table[static_cast<std::size_t>(u)] ^ 1);
                    frontier.push(v);
                } else if (table[static_cast<std::size_t>(v)] ==
                           table[static_cast<std::size_t>(u)]) {
                    ok = false;
                    break;
                }
            }
        }
    }

    std::print("{}\n", ok ? "possible" : "impossible");
    return 0;
}
