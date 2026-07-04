// Exercise T09.7 - The Ranger Outposts
// Session: T09.S4, exercise 3 of 3 (2 hours for the three; this is the hard one)
// Theme: chapter 09 (LCA by binary lifting; distances on a weighted
//   tree), with chapter 01's overflow discipline
// Tags: chapter-09, chapter-01, bfs
//
// MODEL: in a tree there is exactly one simple path between any two
//   vertices, so dist(u, v) is well defined and decomposes at the
//   lowest common ancestor: dist(u, v) = depth(u) + depth(v)
//   - 2 * depth(lca(u, v)), where depth is the WEIGHTED distance from
//   the root. The whole problem is computing lca fast.
// MATH: root the tree (BFS from outpost 1), record parent[0][v] and
//   weighted depth. Binary lifting: parent[j][v] = the 2^j-th
//   ancestor, filled by parent[j][v] = parent[j-1][parent[j-1][v]].
//   lca(u, v): lift the deeper vertex by the binary expansion of the
//   level difference, then lift both in lockstep from the highest
//   power down, stepping only when the 2^j-th ancestors DIFFER; the
//   common parent at the end is the LCA.
// PROOF: lifting correctness is induction on j (a 2^j step is two
//   2^(j-1) steps). For the lockstep phase, the invariant is "u != v
//   and lca is strictly above both": stepping when ancestors differ
//   preserves it (the LCA is never undershot, since equal 2^j-th
//   ancestors mean the LCA is at most 2^j above - we skip that step
//   and halve), and after the j = 0 pass both sit one edge below the
//   LCA. The distance identity holds because the u-v path is the
//   concatenation of the u-to-lca and lca-to-v paths, each measured
//   by depth differences along root paths that share the root-to-lca
//   prefix - the 2*depth(lca) cancels exactly that shared prefix,
//   twice.
// COMPLEXITY: O(n log n) preprocessing (LOG = 18 levels at 2e5),
//   O(log n) per query: ~3.6e6 table entries, ~3.6e6 query steps.
//   Per-query BFS would be O(nq) = 4e10 - the wrong tool, loudly.
// TYPES: depths sum up to 2e5 edges of 1e9: 2e14, long long for
//   depth and answers - the levels and indices stay int. The lifting
//   table is the memory item: 18 * 2e5 ints = 14 MB as int, 28 MB as
//   long long ancestors - ancestors are vertex IDS, so int, and the
//   depth array alone is long long. Mixing those two widths
//   deliberately is the chapter-01 lesson embedded here.
// ALTERNATIVES: Euler tour + sparse table gives O(1) queries after
//   O(n log n) build (more code, same exam grade at q = 2e5); HLD
//   answers path AGGREGATES (max edge, sums under updates) where
//   lifting only answers ancestors; offline Tarjan LCA with DSU is
//   O(n + q alpha) and unbeatable when queries arrive in a file, as
//   here - implemented online anyway, because dispatch's radio does
//   not batch.
// EDGE CASES: u == v (0); query touching the root; a path-shaped
//   forest road system (depth 2e5 - BFS build avoids recursion);
//   star-shaped (all LCAs at the hub); maximum depths with maximum
//   weights (the 2e14 drill).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o outposts t09-s4-3-ranger-outposts.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feoutposts.exe t09-s4-3-ranger-outposts.cpp

#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<std::vector<std::pair<int, long long>>> adj(static_cast<std::size_t>(n) + 1);
    for (int e = 0; e + 1 < n; ++e) {
        int a = 0;
        int b = 0;
        long long d = 0;
        std::cin >> a >> b >> d;
        adj[static_cast<std::size_t>(a)].emplace_back(b, d);
        adj[static_cast<std::size_t>(b)].emplace_back(a, d);
    }

    constexpr int kLog = 18;  // 2^18 > 2e5
    std::vector<std::vector<int>> up(kLog, std::vector<int>(static_cast<std::size_t>(n) + 1, 0));
    std::vector<int> level(static_cast<std::size_t>(n) + 1, -1);
    std::vector<long long> depth(static_cast<std::size_t>(n) + 1, 0);

    std::queue<int> frontier;  // BFS build: no recursion on deep trees
    level[1] = 0;
    up[0][1] = 1;  // root's parent: itself (lifting past the root stalls there)
    frontier.push(1);
    while (!frontier.empty()) {
        const int u = frontier.front();
        frontier.pop();
        for (const auto& [v, d] : adj[static_cast<std::size_t>(u)]) {
            if (level[static_cast<std::size_t>(v)] != -1) continue;
            level[static_cast<std::size_t>(v)] = level[static_cast<std::size_t>(u)] + 1;
            depth[static_cast<std::size_t>(v)] = depth[static_cast<std::size_t>(u)] + d;
            up[0][static_cast<std::size_t>(v)] = u;
            frontier.push(v);
        }
    }
    for (int j = 1; j < kLog; ++j) {
        for (int v = 1; v <= n; ++v) {
            up[static_cast<std::size_t>(j)][static_cast<std::size_t>(v)] =
                up[static_cast<std::size_t>(j) - 1]
                  [static_cast<std::size_t>(up[static_cast<std::size_t>(j) - 1]
                                              [static_cast<std::size_t>(v)])];
        }
    }

    auto lca = [&](int u, int v) {
        if (level[static_cast<std::size_t>(u)] < level[static_cast<std::size_t>(v)]) {
            std::swap(u, v);
        }
        int diff = level[static_cast<std::size_t>(u)] - level[static_cast<std::size_t>(v)];
        for (int j = 0; j < kLog; ++j) {
            if ((diff >> j) & 1) u = up[static_cast<std::size_t>(j)][static_cast<std::size_t>(u)];
        }
        if (u == v) return u;
        for (int j = kLog - 1; j >= 0; --j) {
            const int au = up[static_cast<std::size_t>(j)][static_cast<std::size_t>(u)];
            const int av = up[static_cast<std::size_t>(j)][static_cast<std::size_t>(v)];
            if (au != av) {
                u = au;
                v = av;
            }
        }
        return up[0][static_cast<std::size_t>(u)];
    };

    int q = 0;
    std::cin >> q;
    std::string out;
    out.reserve(static_cast<std::size_t>(q) * 12);
    while (q-- > 0) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        const int w = lca(u, v);
        const long long dist = depth[static_cast<std::size_t>(u)] +
                               depth[static_cast<std::size_t>(v)] -
                               2 * depth[static_cast<std::size_t>(w)];
        out += std::to_string(dist);
        out.push_back('\n');
    }
    std::fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
