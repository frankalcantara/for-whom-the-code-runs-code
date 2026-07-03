// Exercise T13.5 - The Scouts' Reckoning
// Session: T13.S4, exercise 1 of 3 (2 hours for all three)
// Theme: chapter 13 (sweep with coordinate compression), with
//   chapter 06's Fenwick tree and chapter 04's sort
// Tags: chapter-13, chapter-06, chapter-04, sorting, fenwick-tree, coordinate-compression
//
// MODEL: count pairs with x_i < x_j and y_i < y_j - strict 2D
//   dominance.
// MATH: sort by x ascending. Sweep left to right, maintaining a
//   Fenwick tree over COMPRESSED y that counts the scouts already
//   passed. For each scout, the partners are exactly the already-
//   passed scouts with strictly smaller y - a prefix query.
//   Strictness in x is the catch: scouts with EQUAL x must not see
//   each other, so the sweep processes each equal-x group's
//   queries first and only then inserts the whole group.
// PROOF: pairs are counted once, at their right (larger-x)
//   member, when the left member is already in the tree - which by
//   the group discipline happens iff x is STRICTLY smaller. The
//   prefix query enforces y strictly smaller (query up to
//   rank(y) - 1). Compression is order-preserving, so ranks
//   compare exactly as the original coordinates.
// COMPLEXITY: O(n log n) - sort, compression, and 2n Fenwick
//   operations of O(log n). O(n) memory.
// TYPES: the count reaches n(n-1)/2 ~ 2e10: long long, the
//   exercise's standing landmine. Coordinates never enter
//   arithmetic - only comparisons and ranks - so 1e9 inputs are
//   inert. Fenwick cells hold counts <= n: int suffices, int32
//   used.
// ALTERNATIVES: merge-sort counting (the inversion-count adapted)
//   - same bound, no Fenwick, more delicate with the double
//   strictness; a policy tree / order statistic set - same idea,
//   library-dependent; O(n^2) - 4e10, dead.
// EDGE CASES: all scouts on one vertical or one horizontal line
//   (0); duplicate points (count nothing together - equal x
//   handles it); n = 1 (0); answer at the 2e10 scale (long long
//   drill); a strictly increasing staircase (n(n-1)/2, every
//   pair).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o scouts t13-s4-1-scouts-reckoning.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace {

class Fenwick {
   public:
    explicit Fenwick(std::size_t n) : tree_(n + 1, 0) {}

    void add(std::size_t i) {  // 1-based
        for (; i < tree_.size(); i += i & (~i + 1)) ++tree_[i];
    }

    long long prefix(std::size_t i) const {  // sum of [1, i]
        long long s = 0;
        for (; i > 0; i -= i & (~i + 1)) s += tree_[i];
        return s;
    }

   private:
    std::vector<std::int32_t> tree_;
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;
    std::vector<std::pair<long long, long long>> pts(n);
    for (auto& [x, y] : pts) std::cin >> x >> y;

    std::vector<long long> ys(n);
    for (std::size_t i = 0; i < n; ++i) ys[i] = pts[i].second;
    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    const auto rank = [&](long long y) {  // 1-based rank
        return static_cast<std::size_t>(
                   std::lower_bound(ys.begin(), ys.end(), y) - ys.begin()) +
               1;
    };

    std::sort(pts.begin(), pts.end());
    Fenwick tree(ys.size());
    long long count = 0;
    std::size_t i = 0;
    while (i < n) {
        std::size_t j = i;
        while (j < n && pts[j].first == pts[i].first) ++j;
        for (std::size_t k = i; k < j; ++k) {       // query the group first
            count += tree.prefix(rank(pts[k].second) - 1);
        }
        for (std::size_t k = i; k < j; ++k) {       // then insert it
            tree.add(rank(pts[k].second));
        }
        i = j;
    }
    std::print("{}\n", count);
    return 0;
}
