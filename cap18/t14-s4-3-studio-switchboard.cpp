// Exercise T14.7 - The Studio Switchboard
// Session: T14.S4, exercise 3 of 3 (2 hours for all three)
// Theme: chapter 14 (Konig's theorem), using chapter 09's
//   bipartite matching and alternating paths
// Tags: chapter-14, chapter-09
//
// MODEL: minimum vertex cover in a bipartite graph. Microphones are
//   the left side, channels the right side, noisy connections are
//   edges. A tag covers every incident noisy connection.
// MATH: first compute a maximum matching. From every unmatched left
//   vertex, traverse alternating paths: unmatched edges from left to
//   right, matched edges from right to left. Let Z be the vertices
//   reached. Konig's construction gives a minimum cover:
//   (Left \ Z) union (Right intersect Z).
// PROOF: Konig's theorem. The alternating-search construction from
//   a maximum matching produces a cover because an uncovered edge
//   would imply either a new reachable right vertex or a reachable
//   matched left vertex, contradicting the final Z classification.
//   Its size equals the maximum matching size: every matched edge is
//   covered exactly once by the constructed set. Any vertex cover
//   has size at least the size of a matching, so equality proves
//   optimality.
// COMPLEXITY: Kuhn's algorithm costs O(a * m) here, at most
//   1000 * 20000 edge visits in the intended limits. The alternating
//   BFS is O(a + b + m). Memory is O(a + b + m).
// TYPES: indices fit int; the answer is at most min(a, b) and also
//   at most 1000. No sums or products threaten overflow.
// ALTERNATIVES: Dinic on the standard source-left-right-sink network
//   also works and is often faster; Hopcroft-Karp improves matching
//   asymptotically; brute force covers 2^(a+b) choices and is not a
//   plan so much as a resignation letter.
// EDGE CASES: m = 0 (answer 0); a perfect matching (cover size is
//   the smaller side when balanced); isolated microphones or
//   channels (never forced into the cover); duplicate edges are
//   excluded by the statement; disconnected graphs are handled by
//   the traversal from all unmatched left vertices.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o switchboard t14-s4-3-studio-switchboard.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <queue>
#include <vector>

namespace {

class BipartiteCover {
   public:
    BipartiteCover(int left, int right)
        : adj_(left),
          match_left_(left, -1),
          match_right_(right, -1) {}

    void add_edge(int left, int right) { adj_[left].push_back(right); }

    int minimum_vertex_cover_size() {
        maximum_matching();
        std::vector<char> seen_left(adj_.size(), 0);
        std::vector<char> seen_right(match_right_.size(), 0);
        alternating_reachable(seen_left, seen_right);

        int cover = 0;
        for (std::size_t i = 0; i < seen_left.size(); ++i) {
            if (!seen_left[i]) ++cover;
        }
        for (std::size_t j = 0; j < seen_right.size(); ++j) {
            if (seen_right[j]) ++cover;
        }
        return cover;
    }

   private:
    bool try_left(int left, std::vector<char>& seen_right) {
        for (const int right : adj_[left]) {
            if (seen_right[right]) continue;
            seen_right[right] = 1;
            if (match_right_[right] == -1 ||
                try_left(match_right_[right], seen_right)) {
                match_left_[left] = right;
                match_right_[right] = left;
                return true;
            }
        }
        return false;
    }

    void maximum_matching() {
        std::vector<char> seen_right(match_right_.size(), 0);
        for (std::size_t left = 0; left < adj_.size(); ++left) {
            std::fill(seen_right.begin(), seen_right.end(), 0);
            try_left(static_cast<int>(left), seen_right);
        }
    }

    void alternating_reachable(std::vector<char>& seen_left,
                               std::vector<char>& seen_right) const {
        std::queue<int> queue;
        for (std::size_t left = 0; left < match_left_.size(); ++left) {
            if (match_left_[left] == -1) {
                seen_left[left] = 1;
                queue.push(static_cast<int>(left));
            }
        }

        while (!queue.empty()) {
            const int left = queue.front();
            queue.pop();
            for (const int right : adj_[left]) {
                if (match_left_[left] == right || seen_right[right]) {
                    continue;
                }
                seen_right[right] = 1;
                const int next_left = match_right_[right];
                if (next_left != -1 && !seen_left[next_left]) {
                    seen_left[next_left] = 1;
                    queue.push(next_left);
                }
            }
        }
    }

    std::vector<std::vector<int>> adj_;
    std::vector<int> match_left_;
    std::vector<int> match_right_;
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int a = 0;
    int b = 0;
    int m = 0;
    std::cin >> a >> b >> m;

    BipartiteCover solver(a, b);
    for (int e = 0; e < m; ++e) {
        int i = 0;
        int j = 0;
        std::cin >> i >> j;
        solver.add_edge(i - 1, j - 1);
    }

    std::print("{}\n", solver.minimum_vertex_cover_size());
    return 0;
}
