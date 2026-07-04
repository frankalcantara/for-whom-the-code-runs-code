// Exercise A4.5 - The Auction Paddles
// Session: A4, exercise 5 of 6 (6 hours for the evaluation)
// Theme: chapter 06 revision (Fenwick tree as a dynamic multiset with
//   order statistics), with chapter 04's offline coordinate compression
// Tags: chapter-06, chapter-04, binary-search, sorting, prefix-sum, fenwick-tree, coordinate-compression
//
// MODEL: a dynamic multiset of bid amounts under insert, erase-one and
//   k-th smallest. A Fenwick tree over VALUE space, holding counts,
//   supports all three: insert = +1 at the value's index, erase = -1,
//   k-th = the descending binary lifting walk over the implicit tree.
// MATH: values reach 1e9 but only <= q distinct amounts ever appear,
//   and all queries are known up front: read everything, sort the
//   amounts, compress to ranks 1..m (Chapter 4), then replay the
//   events against a Fenwick of size m. kth(k): starting with pos = 0
//   and the highest power of two <= m, greedily extend pos by each
//   power whose prefix count stays < k, consuming that count; pos + 1
//   is the first index with cumulative count >= k.
// PROOF: Fenwick prefix sums are exact under point updates (chapter
//   invariant); for the walk, induction over descending powers
//   maintains "count(1..pos) < k and the k-th lies in (pos,
//   pos + step]": the step is taken exactly when it preserves the
//   first half, and when the loop ends with step 0 the interval has
//   width 1, so pos + 1 is the k-th. Each test uses the node's stored
//   subtree sum, never a full prefix query - that is what makes the
//   walk O(log m) instead of O(log^2 m) binary search over queries.
// COMPLEXITY: O(q log q) total - sort for compression, then O(log m)
//   per event. Memory O(q). The log^2 variant (binary search an index,
//   prefix-query each probe) also passes at 2e5 and is the honest
//   fallback if the walk's indices feel slippery at hour five.
// TYPES: counts fit int (at most 2e5 bids stand); compressed indices
//   int; the printed amounts are the original values, up to 1e9, which
//   fit int but are stored as long long-free int deliberately - the
//   board prints values, it never sums them, so no width anxiety.
// ALTERNATIVES: a balanced BST with subtree sizes (no std container
//   exposes one portably); two heaps track only ONE quantile, not a
//   moving k; std::multiset finds k-th in O(k) - the trap this
//   exercise is fishing for; a policy tree (GNU pb_ds) solves it
//   directly where allowed - know your judge.
// EDGE CASES: k-th right after erasures collapse the board; duplicate
//   amounts (counts > 1 at one index, the walk handles multiplicity);
//   k equal to the number of standing bids (maximum); alternating
//   insert/erase of the same value; the example's withdrawal changing
//   the answer to the same query.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o paddles a4-05-auction-paddles.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fepaddles.exe a4-05-auction-paddles.cpp

#include <algorithm>
#include <bit>
#include <iostream>
#include <print>
#include <vector>

namespace {

class FenwickCounts {
public:
    explicit FenwickCounts(int size) : tree_(static_cast<std::size_t>(size) + 1, 0) {}

    void add(int index, int delta) {  // 1-based
        for (int i = index; i < static_cast<int>(tree_.size()); i += i & (-i)) {
            tree_[static_cast<std::size_t>(i)] += delta;
        }
    }

    // Smallest index whose cumulative count reaches k (k >= 1 and valid).
    int kth(int k) const {
        int pos = 0;
        int step = std::bit_floor(tree_.size() - 1);
        for (; step > 0; step >>= 1) {
            const int next = pos + step;
            if (next < static_cast<int>(tree_.size()) &&
                tree_[static_cast<std::size_t>(next)] < k) {
                pos = next;
                k -= tree_[static_cast<std::size_t>(pos)];
            }
        }
        return pos + 1;
    }

private:
    std::vector<int> tree_;
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q = 0;
    std::cin >> q;

    std::vector<int> type(static_cast<std::size_t>(q));
    std::vector<int> arg(static_cast<std::size_t>(q));
    std::vector<int> amounts;
    for (int i = 0; i < q; ++i) {
        std::cin >> type[static_cast<std::size_t>(i)] >> arg[static_cast<std::size_t>(i)];
        if (type[static_cast<std::size_t>(i)] != 3) {
            amounts.push_back(arg[static_cast<std::size_t>(i)]);
        }
    }

    std::ranges::sort(amounts);
    const auto dup = std::ranges::unique(amounts);
    amounts.erase(dup.begin(), dup.end());
    auto rank_of = [&amounts](int v) {
        return static_cast<int>(std::ranges::lower_bound(amounts, v) - amounts.begin()) + 1;
    };

    FenwickCounts board(static_cast<int>(amounts.size()));
    for (int i = 0; i < q; ++i) {
        const int t = type[static_cast<std::size_t>(i)];
        const int v = arg[static_cast<std::size_t>(i)];
        if (t == 1) {
            board.add(rank_of(v), +1);
        } else if (t == 2) {
            board.add(rank_of(v), -1);
        } else {
            const int idx = board.kth(v);
            std::print("{}\n", amounts[static_cast<std::size_t>(idx) - 1]);
        }
    }
    return 0;
}
