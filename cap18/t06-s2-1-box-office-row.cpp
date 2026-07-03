// Exercise T06.2 - The Box Office Row
// Session: T06.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 06 (Fenwick tree; point update, range sum, interleaved)
// Tags: chapter-06, prefix-sum, fenwick-tree, number-theory
//
// MODEL: an array under point-add and range-sum, both online. This is
//   the Fenwick contract verbatim: prefix sums that survive updates.
// MATH: tree[i] stores the sum of the block of length lowbit(i) = i & -i
//   ending at i. prefix(i) walks i -> i - lowbit(i) down to zero, summing
//   blocks; add(i, k) walks i -> i + lowbit(i) up to n, fixing every
//   block that contains i. sum(l, r) = prefix(r) - prefix(l - 1).
// PROOF: the blocks visited by prefix(i) partition [1, i]: each step
//   strips the lowest set bit of i, so the visited blocks are disjoint
//   and their union is exactly [1, i] (induction on the number of set
//   bits). A block ending at j with length lowbit(j) contains i iff j is
//   reached from i by the add walk, so add fixes precisely the blocks
//   whose sums changed: the two walks are mutually inverse decompositions.
// COMPLEXITY: O(log n) per event (the walks visit one node per bit),
//   O(n) build by n point-adds in O(n log n) (a linear build exists, the
//   chapter shows it; with n = 2e5 the difference does not pay its own
//   code). Total O((n + q) log n), memory O(n). Prefix sums alone break
//   under updates (O(n) rebuild each); a segment tree matches the bounds
//   with about four times the memory and code for power this problem
//   does not need (no range update, no non-invertible operator).
// TYPES: the grand total reaches n * 1e9 + q * 1e9 = 4e14: long long in
//   the tree cells and in every prefix accumulation. Window indices fit
//   in int.
// ALTERNATIVES: sqrt decomposition (O(sqrt n) per op, simpler theory,
//   slower); ordered map of dirty windows plus periodic flush (an
//   amortization with no better bound). Fenwick is 15 lines and exact.
// EDGE CASES: query of a single window (l = r); query of the full row;
//   repeated updates to the same window; a_i = 0 start; the 4e14 total.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o boxoffice t06-s2-1-box-office-row.cpp

#include <iostream>
#include <print>
#include <vector>

class Fenwick {
public:
    explicit Fenwick(int n) : tree_(n + 1, 0) {}

    void add(int i, long long k) {
        for (; i < static_cast<int>(tree_.size()); i += i & -i) tree_[i] += k;
    }

    long long prefix(int i) const {
        long long s = 0;
        for (; i > 0; i -= i & -i) s += tree_[i];
        return s;
    }

    long long sum(int l, int r) const { return prefix(r) - prefix(l - 1); }

private:
    std::vector<long long> tree_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    Fenwick fen(n);
    for (int i = 1; i <= n; ++i) {
        long long a = 0;
        std::cin >> a;
        fen.add(i, a);
    }

    while (q-- > 0) {
        int type = 0;
        std::cin >> type;
        if (type == 1) {
            int i = 0;
            long long k = 0;
            std::cin >> i >> k;
            fen.add(i, k);
        } else {
            int l = 0;
            int r = 0;
            std::cin >> l >> r;
            std::print("{}\n", fen.sum(l, r));
        }
    }
    return 0;
}
