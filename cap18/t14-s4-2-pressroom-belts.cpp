// Exercise T14.6 - The Pressroom Belts
// Session: T14.S4, exercise 2 of 3 (2 hours for all three)
// Theme: chapter 14 (parameterized branching for vertex cover)
// Tags: chapter-14
//
// MODEL: given an undirected graph, decide whether there is a
//   vertex cover of size at most k: a set of vertices touching every
//   edge.
// MATH: if an uncovered edge (u, v) remains, every valid cover must
//   choose u or v. Branch on those two choices and reduce k by one.
//   A matching among uncovered edges gives a lower bound: pairwise
//   disjoint edges require distinct chosen endpoints, so if a found
//   matching already has size greater than the remaining budget, the
//   branch cannot succeed.
// PROOF: induction on k. The base case with no uncovered edges is
//   true; with k < 0 it is false. For any uncovered edge (u, v), any
//   feasible cover must contain u or v, so the recursive branches are
//   exhaustive. Adding one endpoint preserves the invariant that the
//   selected set is the partial cover. The matching prune discards
//   only branches whose uncovered graph needs more vertices than the
//   remaining budget can supply.
// COMPLEXITY: O(2^k * m) in the branching tree, with an O(m) scan
//   and lower-bound computation per node. With k <= 25 and m <= 200,
//   the parameter, not n, is the limit being trained. Memory is
//   O(n + m) plus recursion depth O(k).
// TYPES: n <= 60 fits in one uint64_t mask of selected machines.
//   Edge endpoints and k fit int. No arithmetic grows beyond these
//   limits.
// ALTERNATIVES: DP over all subsets of vertices is O(2^n), useless
//   at n = 60; maximum matching solves the problem exactly only in
//   bipartite graphs; the 2-approximation is fast but answers the
//   wrong yes/no question.
// EDGE CASES: m = 0 (YES, choose nobody); k = 0 with at least one
//   edge (NO); a star (one center is enough); a matching of k + 1
//   edges (NO by the lower bound); n = 1 with no self-loops.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o belts t14-s4-2-pressroom-belts.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febelts.exe t14-s4-2-pressroom-belts.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace {

class VertexCoverDecision {
   public:
    explicit VertexCoverDecision(std::vector<std::pair<int, int>> edges)
        : edges_(std::move(edges)) {}

    bool possible(int k) { return search(0, k); }

   private:
    bool covered(std::uint64_t chosen, int u, int v) const {
        return ((chosen >> u) & 1U) != 0 || ((chosen >> v) & 1U) != 0;
    }

    int matching_lower_bound(std::uint64_t chosen) const {
        std::uint64_t used = 0;
        int count = 0;
        for (const auto [u, v] : edges_) {
            if (covered(chosen, u, v)) continue;
            const std::uint64_t endpoints =
                (std::uint64_t{1} << u) | (std::uint64_t{1} << v);
            if ((used & endpoints) == 0) {
                used |= endpoints;
                ++count;
            }
        }
        return count;
    }

    bool first_uncovered_edge(std::uint64_t chosen, int& u, int& v) const {
        for (const auto [a, b] : edges_) {
            if (!covered(chosen, a, b)) {
                u = a;
                v = b;
                return true;
            }
        }
        return false;
    }

    bool search(std::uint64_t chosen, int remaining) const {
        int u = -1;
        int v = -1;
        if (!first_uncovered_edge(chosen, u, v)) return true;
        if (remaining == 0) return false;
        if (matching_lower_bound(chosen) > remaining) return false;

        return search(chosen | (std::uint64_t{1} << u), remaining - 1) ||
               search(chosen | (std::uint64_t{1} << v), remaining - 1);
    }

    std::vector<std::pair<int, int>> edges_;
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    int k = 0;
    std::cin >> n >> m >> k;

    std::vector<std::pair<int, int>> edges;
    edges.reserve(static_cast<std::size_t>(m));
    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        edges.emplace_back(u - 1, v - 1);
    }

    VertexCoverDecision solver(std::move(edges));
    std::print("{}\n", solver.possible(k) ? "YES" : "NO");
    return 0;
}
