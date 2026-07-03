// Exercise A8.5 - The Turntable Tickets
// Session: A8, exercise 5 of 10
// Theme: chapter 06 (Fenwick tree with coordinate compression)
// Tags: chapter-06, sorting, fenwick-tree, coordinate-compression
//
// MODEL: count inversions in an integer sequence.
// MATH: scan left to right. Before a[i], the Fenwick tree stores
//   counts of previous values by compressed rank. Previous values
//   greater than a[i] equal seen_so_far - count(previous <= a[i]).
// PROOF: each inversion is counted exactly when its right endpoint
//   is scanned. Compression preserves order, so rank comparisons are
//   equivalent to value comparisons.
// COMPLEXITY: O(n log n) time and O(n) memory.
// TYPES: the answer can reach n(n-1)/2, so int64_t is used.
// ALTERNATIVES: merge-sort counting has the same bound; O(n^2) is
//   too slow.
// EDGE CASES: equal values; sorted input; reverse sorted input.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o tickets a8-05-turntable-tickets.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

class Fenwick {
   public:
    explicit Fenwick(int n) : tree_(static_cast<std::size_t>(n + 1), 0) {}
    void add(int i, int delta) {
        for (; i < static_cast<int>(tree_.size()); i += i & -i) tree_[i] += delta;
    }
    int sum(int i) const {
        int result = 0;
        for (; i > 0; i -= i & -i) result += tree_[i];
        return result;
    }
   private:
    std::vector<int> tree_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<int> a(n);
    for (int& x : a) std::cin >> x;
    std::vector<int> sorted = a;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    Fenwick tree(static_cast<int>(sorted.size()));
    std::int64_t inversions = 0;
    for (int i = 0; i < n; ++i) {
        const int rank = static_cast<int>(
            std::lower_bound(sorted.begin(), sorted.end(), a[i]) - sorted.begin()) + 1;
        inversions += i - tree.sum(rank);
        tree.add(rank, 1);
    }
    std::print("{}\n", inversions);
    return 0;
}
