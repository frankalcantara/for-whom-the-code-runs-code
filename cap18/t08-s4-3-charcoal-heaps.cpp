// Exercise T08.7 - The Charcoal Heaps
// Session: T08.S4, exercise 3 of 3 (2 hours for the three; this is the hard one)
// Theme: chapter 08 (interval DP), using chapter 02 (prefix sums)
// Tags: chapter-08, chapter-02, prefix-sum, dynamic-programming, interval-dp
//
// MODEL: the classic stone-merging interval DP. Whatever the merge
//   order, the heaps combined into one final heap always form a
//   contiguous block of the original row (merging is adjacent-only),
//   so "minimum effort to fully merge block [l, r]" is a
//   self-contained subproblem - the defining insight of interval DP.
// MATH: dp[l][r] = min over split points k in [l, r) of
//   dp[l][k] + dp[k+1][r] + S(l, r), where S(l, r) is the block's
//   total weight (prefix sums, Chapter 2) - the last merge of the
//   block always pays the whole block's weight. Base: dp[l][l] = 0.
//   Fill by increasing block length; every term on the right is a
//   shorter block.
// PROOF: consider an optimal merge order for [l, r] and its LAST
//   merge: it combines a fully merged [l, k] with a fully merged
//   [k+1, r] for some k (the two operands are contiguous blocks
//   partitioning [l, r], because adjacency is preserved throughout).
//   The two sub-orders must each be optimal for their blocks (cut and
//   paste), and the last merge costs S(l, r) regardless of k. Taking
//   the min over all k is therefore exhaustive; induction on block
//   length closes the argument.
// COMPLEXITY: O(n^3) time - n^2/2 blocks, O(n) splits each, about
//   2.1e7 inner iterations at n = 400 - and O(n^2) memory (160k
//   long long entries, ~1.3 MB). Knuth's optimization cuts this to
//   O(n^2) when needed; at n = 400 it is not needed.
// TYPES: a block's weight reaches 400 * 1e9 = 4e11, and total effort
//   is bounded by (n-1) merges each paying at most 4e11: ~1.6e14.
//   Everything long long; the prefix-sum array especially, where the
//   4e11 already overflows int inside the FIRST addition.
// ALTERNATIVES: greedy "always merge the lightest adjacent pair"
//   fails (4 1 1 4: greedy is optimal here, but 1 5 1 1... at some
//   sizes it misorders splits; no exchange argument survives). The
//   Huffman merge (any pair, not just adjacent) is a different, easier
//   problem - the adjacency constraint is what buys the DP its O(n^3).
//   Memoized recursion matches the bound with stack depth n.
// EDGE CASES: n = 1 (no merge, 0); n = 2 (one merge, w_1 + w_2);
//   equal weights (balanced splitting wins); a giant heap at one end
//   (the DP must delay touching it); maximum n and weights (the
//   overflow drill in TYPES).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o heaps t08-s4-3-charcoal-heaps.cpp

#include <algorithm>
#include <iostream>
#include <limits>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<long long> prefix(static_cast<std::size_t>(n) + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long w = 0;
        std::cin >> w;
        prefix[static_cast<std::size_t>(i) + 1] = prefix[static_cast<std::size_t>(i)] + w;
    }
    auto block_sum = [&prefix](int l, int r) {
        return prefix[static_cast<std::size_t>(r) + 1] - prefix[static_cast<std::size_t>(l)];
    };

    const std::size_t un = static_cast<std::size_t>(n);
    std::vector<std::vector<long long>> dp(un, std::vector<long long>(un, 0));
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            const int r = l + len - 1;
            long long best = std::numeric_limits<long long>::max();
            for (int k = l; k < r; ++k) {
                best = std::min(best, dp[static_cast<std::size_t>(l)][static_cast<std::size_t>(k)] +
                                          dp[static_cast<std::size_t>(k) + 1][static_cast<std::size_t>(r)]);
            }
            dp[static_cast<std::size_t>(l)][static_cast<std::size_t>(r)] = best + block_sum(l, r);
        }
    }
    std::print("{}\n", dp[0][un - 1]);
    return 0;
}
