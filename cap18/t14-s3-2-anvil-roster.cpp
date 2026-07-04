// Exercise T14.4 - The Anvil Roster
// Session: T14.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 09's bipartite matching, reused inside chapter
//   14's world of hard problems and structural reductions
// Tags: chapter-09, chapter-14, dfs
//
// MODEL: maximum matching in a bipartite graph - apprentices on
//   one side, anvils on the other, qualifications as edges.
// MATH: grow the matching one augmenting path at a time. For each
//   apprentice, DFS for an ALTERNATING path: a free anvil, or an
//   anvil whose current holder can be re-seated on another anvil
//   (recursively). Flipping the path's edges adds one match.
//   When no augmenting path exists from any unmatched apprentice,
//   the matching is maximum (Berge's theorem).
// PROOF: Berge - a matching is maximum iff it admits no
//   augmenting path. If a larger matching M' existed, the
//   symmetric difference M xor M' is a union of paths and even
//   cycles with max degree 2; |M'| > |M| forces some component
//   with more M'-edges than M-edges, which is exactly an
//   M-augmenting path - contradicting the DFS's failure to find
//   one. Each augmentation raises the size by one and never
//   un-matches anyone, so termination at the maximum is reached
//   in <= a rounds.
// COMPLEXITY: O(V * E) = 500 * 1e4 = 5e6 worst case - Kuhn's
//   bound; the visited[] reset per apprentice keeps each round
//   linear in E. Memory O(V + E).
// TYPES: all indices fit int32; the answer <= min(a, b) <= 500.
//   Nothing here can overflow anything - the exercise's rest stop
//   between the chapter's bitmask cliffs.
// ALTERNATIVES: Hopcroft-Karp - O(E sqrt V), the upgrade when
//   V reaches 1e5, double the code for no gain at 500; max flow
//   with unit capacities (Dinic) - same asymptotics as
//   Hopcroft-Karp here and the chapter 9 connection worth
//   remembering; greedy matching - a 1/2-approximation, the thing
//   Kuhn's recursion repairs.
// EDGE CASES: m = 0 (0); an apprentice with no qualifications
//   (skipped by the DFS trivially); perfect matching on one side
//   (answer min(a,b)); duplicate qualification lines tolerated
//   (parallel edges change nothing); the star (one apprentice,
//   many anvils - answer 1).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o roster t14-s3-2-anvil-roster.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feroster.exe t14-s3-2-anvil-roster.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

class Kuhn {
   public:
    Kuhn(std::size_t left, std::size_t right)
        : adj_(left), match_right_(right, -1) {}

    void add_edge(std::size_t l, std::size_t r) { adj_[l].push_back(r); }

    int solve() {
        int matched = 0;
        std::vector<char> visited(match_right_.size(), 0);
        for (std::size_t l = 0; l < adj_.size(); ++l) {
            std::fill(visited.begin(), visited.end(), 0);
            if (try_apprentice(l, visited)) ++matched;
        }
        return matched;
    }

   private:
    bool try_apprentice(std::size_t l, std::vector<char>& visited) {
        for (const std::size_t r : adj_[l]) {
            if (visited[r]) continue;
            visited[r] = 1;
            if (match_right_[r] == -1 ||
                try_apprentice(static_cast<std::size_t>(match_right_[r]),
                               visited)) {
                match_right_[r] = static_cast<std::int32_t>(l);
                return true;
            }
        }
        return false;
    }

    std::vector<std::vector<std::size_t>> adj_;
    std::vector<std::int32_t> match_right_;
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t a = 0;
    std::size_t b = 0;
    std::size_t m = 0;
    std::cin >> a >> b >> m;

    Kuhn kuhn(a, b);
    for (std::size_t e = 0; e < m; ++e) {
        std::size_t i = 0;
        std::size_t j = 0;
        std::cin >> i >> j;
        kuhn.add_edge(i - 1, j - 1);
    }
    std::print("{}\n", kuhn.solve());
    return 0;
}
