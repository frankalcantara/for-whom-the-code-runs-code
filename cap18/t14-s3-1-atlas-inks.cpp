// Exercise T14.3 - The Atlas Inks
// Session: T14.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 14 (3-colorability decided by subset enumeration),
//   with chapter 09's bipartiteness BFS doing half the work
// Tags: chapter-14, chapter-09, bfs, bitmask, brute-force
//
// MODEL: is the chromatic number at most 3?
// MATH: a graph is 3-colorable iff some independent set S (the
//   crimson provinces) leaves a bipartite remainder (ochre and
//   blue split the rest). Enumerate all 2^n subsets as candidate
//   S; test independence with precomputed adjacency bitmasks; for
//   independent S, two-color V \ S by BFS, edges within the
//   remainder only.
// PROOF: (=>) a proper 3-coloring's first color class is
//   independent and the other two classes witness bipartiteness
//   of the rest. (<=) paint S crimson - no internal edges by
//   independence - and the remainder's two sides ochre and blue;
//   every edge is either inside the remainder (properly split),
//   or touches S at exactly one end (S independent), so all
//   endpoints differ. The enumeration is exhaustive, so the iff
//   decides exactly.
// COMPLEXITY: O(2^n * (n + m)) worst case: 1e6 * ~230 ~ 2.4e8
//   bit-cheap steps at the limits - heavy but honest, and the
//   independence test kills most masks in a few ANDs. (The
//   classic backtracking colorer is usually faster in practice
//   and exponentially worse on adversarial inputs; this version
//   trades constant factor for a bound you can state.)
// TYPES: adjacency as uint32 masks (n <= 20); BFS colors are
//   small signed integers; nothing grows.
// ALTERNATIVES: plain backtracking with symmetry breaking (fix
//   vertex 1's color) - try it, time it, compare; SAT-style
//   clause propagation - the grown-up tool, out of scope; for TWO
//   inks the whole exercise collapses to chapter 9's bipartite
//   check, which is the remainder test here - the chapters stack.
// EDGE CASES: m = 0 (YES - one ink would do); the complete graph
//   K4 (NO, the example); odd cycle alone (YES - 3 colors handle
//   it; 2 would not); disconnected graphs (BFS per component);
//   n = 1 or 2 (always YES).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o inks t14-s3-1-atlas-inks.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feinks.exe t14-s3-1-atlas-inks.cpp

#include <bit>
#include <cstdint>
#include <iostream>
#include <print>
#include <queue>
#include <vector>

namespace {

// Two-color the vertices OUTSIDE mask using only edges with both
// ends outside; adjacency given as bitmasks.
bool remainder_bipartite(const std::vector<std::uint32_t>& adj,
                         std::uint32_t mask, std::size_t n) {
    std::vector<int> color(n, -1);
    for (std::size_t s = 0; s < n; ++s) {
        if ((mask >> s & 1) || color[s] != -1) continue;
        color[s] = 0;
        std::queue<std::size_t> queue;
        queue.push(s);
        while (!queue.empty()) {
            const std::size_t v = queue.front();
            queue.pop();
            std::uint32_t nbrs = adj[v] & ~mask;
            while (nbrs != 0) {
                const auto u =
                    static_cast<std::size_t>(std::countr_zero(nbrs));
                nbrs &= nbrs - 1;
                if (color[u] == -1) {
                    color[u] = 1 - color[v];
                    queue.push(u);
                } else if (color[u] == color[v]) {
                    return false;
                }
            }
        }
    }
    return true;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;
    std::vector<std::uint32_t> adj(n, 0);
    for (std::size_t e = 0; e < m; ++e) {
        std::size_t u = 0;
        std::size_t v = 0;
        std::cin >> u >> v;
        adj[u - 1] |= std::uint32_t{1} << (v - 1);
        adj[v - 1] |= std::uint32_t{1} << (u - 1);
    }

    const std::uint32_t full = (n == 32) ? ~std::uint32_t{0}
                                         : (std::uint32_t{1} << n) - 1;
    for (std::uint32_t mask = 0; mask <= full; ++mask) {
        bool independent = true;
        for (std::uint32_t rest = mask; rest != 0 && independent;
             rest &= rest - 1) {
            const auto v = static_cast<std::size_t>(std::countr_zero(rest));
            if ((adj[v] & mask) != 0) independent = false;
        }
        if (independent && remainder_bipartite(adj, mask, n)) {
            std::print("YES\n");
            return 0;
        }
    }
    std::print("NO\n");
    return 0;
}
