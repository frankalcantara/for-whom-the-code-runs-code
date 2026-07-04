// Exercise A4.6 - The Mine Galleries
// Session: A4, exercise 6 of 6 (6 hours for the evaluation; integration)
// Theme: chapters 07 + 08 (BFS layering + counting DP on the BFS DAG),
//   with chapter 01's modular-counting hygiene
// Tags: chapter-07, chapter-08, chapter-01, bfs, dag-dp, dynamic-programming, number-theory, modular-arithmetic
//
// MODEL: two questions about one structure. BFS (Chapter 7) computes
//   dist, the shortest-move labeling of the grid; the shortest routes
//   are exactly the paths of the DAG whose edges go from dist = d to
//   dist = d + 1, and counting paths in a DAG is a DP (Chapter 8):
//   ways[v] = sum of ways[u] over BFS-predecessors u of v.
// MATH: process vertices in BFS dequeue order, which sorts them by
//   dist. When relaxing edge (u, v): if v is new, dist[v] = dist[u]+1
//   and ways[v] = ways[u]; if v was already labeled with dist[u] + 1,
//   it is another shortest way in: ways[v] += ways[u] (mod p). Edges
//   to vertices with dist <= dist[u] are not shortest-path edges and
//   add nothing.
// PROOF: by induction on d: when the first vertex of layer d is
//   dequeued, every layer-(d-1) vertex has been dequeued (BFS order),
//   so each layer-d vertex has received exactly one contribution from
//   EACH of its layer-(d-1) neighbors - and shortest paths to v
//   decompose uniquely as (shortest path to a predecessor) + (final
//   edge), a bijection summed over predecessors. No other vertex ever
//   touches ways[v]: a u with dist[u] >= dist[v] cannot relax v.
//   Hence ways[E] counts shortest S-E routes exactly, mod p.
// COMPLEXITY: O(rc) time and memory - BFS plus O(1) work per edge,
//   4e6 edge relaxations at 1000 x 1000. A second pass is not needed:
//   the count rides along with the BFS.
// TYPES: dist <= rc <= 1e6: int. Counts are astronomical (binomial
//   growth) and reduced mod 1e9+7 in long long arithmetic; the sum of
//   up to three predecessor counts stays below 3e9+21 - overflows int,
//   fits long long, the classic one-line trap of counting DPs.
// ALTERNATIVES: running BFS first and a separate topological DP
//   second computes the same numbers in two passes (cleaner to debug,
//   double the traversals); DFS counting without the layering counts
//   ALL simple paths - exponential and wrong. Counting without the
//   modulus overflows for grids as small as 60 x 60 (C(118, 59) ~
//   1e34): the modulus is not decoration.
// EDGE CASES: S adjacent to E (1 1); S walled in (trapped); a single
//   corridor (count 1); the full open grid (counts are binomials -
//   cross-check C(r+c-2, r-1) mod p); equal-length detours around
//   multiple pillars (counts multiply per independent choice).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o mine a4-06-mine-galleries.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femine.exe a4-06-mine-galleries.cpp

#include <iostream>
#include <print>
#include <queue>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int r = 0;
    int c = 0;
    std::cin >> r >> c;

    std::vector<std::string> grid(static_cast<std::size_t>(r));
    int start = -1;
    int exit_shaft = -1;
    for (int i = 0; i < r; ++i) {
        std::cin >> grid[static_cast<std::size_t>(i)];
        for (int j = 0; j < c; ++j) {
            const char ch = grid[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)];
            if (ch == 'S') start = i * c + j;
            if (ch == 'E') exit_shaft = i * c + j;
        }
    }

    constexpr long long kMod = 1'000'000'007;
    const std::size_t cells = static_cast<std::size_t>(r) * static_cast<std::size_t>(c);
    std::vector<int> dist(cells, -1);
    std::vector<long long> ways(cells, 0);
    std::queue<int> frontier;

    dist[static_cast<std::size_t>(start)] = 0;
    ways[static_cast<std::size_t>(start)] = 1;
    frontier.push(start);

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    while (!frontier.empty()) {
        const int id = frontier.front();
        frontier.pop();
        const int i = id / c;
        const int j = id % c;
        for (int k = 0; k < 4; ++k) {
            const int ni = i + dr[k];
            const int nj = j + dc[k];
            if (ni < 0 || ni >= r || nj < 0 || nj >= c) continue;
            if (grid[static_cast<std::size_t>(ni)][static_cast<std::size_t>(nj)] == '#') continue;
            const std::size_t nid = static_cast<std::size_t>(ni) * static_cast<std::size_t>(c) +
                                    static_cast<std::size_t>(nj);
            if (dist[nid] == -1) {
                dist[nid] = dist[static_cast<std::size_t>(id)] + 1;
                ways[nid] = ways[static_cast<std::size_t>(id)];
                frontier.push(static_cast<int>(nid));
            } else if (dist[nid] == dist[static_cast<std::size_t>(id)] + 1) {
                ways[nid] = (ways[nid] + ways[static_cast<std::size_t>(id)]) % kMod;
            }
        }
    }

    const std::size_t e = static_cast<std::size_t>(exit_shaft);
    if (dist[e] == -1) {
        std::print("trapped\n");
    } else {
        std::print("{} {}\n", dist[e], ways[e]);
    }
    return 0;
}
