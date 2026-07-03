// Exercise A5.3 - The Quarrelsome Jury
// Session: A5, exercise 3 of 7 (6 hours for the evaluation)
// Theme: chapter 07 revision (backtracking with bitmask state and
//   feasibility pruning), with chapter 05's bit toolkit on loan
// Tags: chapter-07, chapter-05, bitmask, backtracking, brute-force
//
// MODEL: count the independent sets of size exactly k in a graph of
//   n <= 20 vertices (citizens) whose edges are the feuds. n = 20
//   puts the problem squarely in exhaustive-search territory: the
//   search tree over "take / skip each citizen in order" has at most
//   2^20 leaves, and pruning cuts most of them.
// MATH: each citizen i carries a feud mask conflict[i] (bit j set if
//   i and j feud). Walking citizens in index order with the running
//   mask of chosen citizens, citizen i is admissible iff
//   (chosen_mask & conflict[i]) == 0 - every feud is checked exactly
//   once, against its lower-numbered endpoint's mask when the
//   higher-numbered one is considered (the masks are symmetric, so
//   order does not matter).
// PROOF: the take/skip tree enumerates each subset of citizens exactly
//   once (a subset is the set of "take" decisions; distinct subsets
//   differ at the first index where their decisions differ). The
//   admissibility test ensures precisely the independent-set property
//   for the partial set at every node, and an invalid partial set
//   cannot become valid by adding citizens - feuds do not expire -
//   so pruning the subtree loses no valid jury. The size prune
//   (chosen + remaining < k) discards only subtrees whose every leaf
//   is too small.
// COMPLEXITY: O(2^n) worst case (f = 0 forces visiting every subset
//   of size <= k), in practice far less; the example tree has 11
//   nodes. Memory O(n) recursion. The flat alternative - iterate all
//   2^20 masks, test popcount and feuds - is also fine at this size
//   and is honest about being a loop.
// TYPES: masks fit in a 32-bit unsigned (n <= 20); the count is at
//   most C(20,10) = 184756, comfortably int, declared int64_t out of
//   chapter 01 habit rather than need - the habit is cheaper than the
//   audit.
// ALTERNATIVES: DP over subsets (chapter 14's bitmask DP would count
//   independent sets in O(2^n) too, but is two chapters past this
//   evaluation's window and overkill for counting by size);
//   inclusion-exclusion over feud edges explodes combinatorially;
//   branch and bound adds nothing when the answer itself can be
//   exponential in the worst case.
// EDGE CASES: k = 0 (exactly one jury: the empty one, regardless of
//   feuds); f = 0 (answer is C(n, k)); a citizen feuding with
//   everyone (never seated unless alone); k = n with any feud
//   (answer 0); n = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o jury a5-03-quarrelsome-jury.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

class JuryCounter {
public:
    JuryCounter(int n, int k, std::vector<std::uint32_t> conflict)
        : n_(n), k_(k), conflict_(std::move(conflict)) {}

    std::int64_t count() {
        total_ = 0;
        descend(0, 0, 0U);
        return total_;
    }

private:
    void descend(int index, int chosen, std::uint32_t chosen_mask) {
        if (chosen == k_) {
            ++total_;
            return;
        }
        if (index == n_ || chosen + (n_ - index) < k_) {
            return;  // not enough citizens left to fill the jury
        }
        // Skip citizen `index`.
        descend(index + 1, chosen, chosen_mask);
        // Seat citizen `index` if no chosen citizen feuds with them.
        if ((chosen_mask & conflict_[static_cast<std::size_t>(index)]) == 0U) {
            descend(index + 1, chosen + 1, chosen_mask | (1U << index));
        }
    }

    int n_;
    int k_;
    std::vector<std::uint32_t> conflict_;
    std::int64_t total_ = 0;
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    int f = 0;
    std::cin >> n >> k >> f;

    std::vector<std::uint32_t> conflict(static_cast<std::size_t>(n), 0U);
    for (int i = 0; i < f; ++i) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        conflict[static_cast<std::size_t>(u - 1)] |= 1U << (v - 1);
        conflict[static_cast<std::size_t>(v - 1)] |= 1U << (u - 1);
    }

    JuryCounter counter(n, k, std::move(conflict));
    std::println("{}", counter.count());
    return 0;
}
