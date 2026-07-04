// Exercise A3.5 - The Parade Disorder
// Session: A3, exercise 5 of 5 (6 hours for the evaluation; integration)
// Theme: chapters 04 + 06 + 01 (inversion counting: coordinate
//   compression by sort/unique/lower_bound, a Fenwick tree over ranks,
//   and a count that overflows int by five orders of magnitude)
// Tags: chapter-04, chapter-06, chapter-01, binary-search, sorting, fenwick-tree
//
// MODEL: count pairs i < j with s[i] > s[j] (inversions; ties excluded).
// MATH: sweep the column left to right over compressed ranks. When
//   marcher j arrives, the pairs it closes are the earlier marchers with
//   strictly larger seniority: (number seen so far) - (number with rank
//   <= rank(j)) = j - prefix(rank(j)). A Fenwick tree over ranks
//   maintains the counts: query prefix, then add 1 at rank(j).
// PROOF: every inversion (i, j) is counted exactly once, at the moment
//   its right endpoint j is processed, because i < j means i was already
//   inserted, and s[i] > s[j] means i is outside prefix(rank(j)); ties
//   land inside the prefix and are correctly not counted. No pair is
//   counted elsewhere: the sweep attributes each pair to its unique
//   right endpoint. Summing over j gives the total.
// COMPLEXITY: O(n log n) for compression and for the n queries+updates.
//   Memory O(n). The naive double loop is O(n^2) = 4e10, over budget by
//   a factor of a few hundred.
// TYPES: the maximum count is n(n-1)/2 ~ 2e10, which does not fit in
//   int (max ~2.1e9) — the classic chapter 01 trap planted in plain
//   sight. The accumulator and the Fenwick prefix counts are long long
//   (counts per rank fit in int; the accumulated answer does not).
// ALTERNATIVES: merge-sort counting (same bound, no Fenwick, mutates
//   the array; the chapter 08 merge ceremony will revisit it); a
//   policy-tree / order-statistics set (nonstandard headers, banned at
//   many judges); Mo's algorithm cannot help, there are no range queries.
// EDGE CASES: sorted column (0); reverse-sorted column (n(n-1)/2, the
//   overflow witness); all equal seniorities (0, ties excluded); n = 1;
//   two elements both orders.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o parade a3-05-parade-disorder.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feparade.exe a3-05-parade-disorder.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

class Fenwick {
public:
    explicit Fenwick(int n) : tree_(n + 1, 0) {}

    void add(int i) {
        for (; i < static_cast<int>(tree_.size()); i += i & -i) ++tree_[i];
    }

    long long prefix(int i) const {
        long long s = 0;
        for (; i > 0; i -= i & -i) s += tree_[i];
        return s;
    }

private:
    std::vector<int> tree_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<int> s(n);
    for (auto& x : s) std::cin >> x;

    // coordinate compression: rank in [1, k]
    std::vector<int> sorted = s;
    std::ranges::sort(sorted);
    auto dup = std::ranges::unique(sorted);
    sorted.erase(dup.begin(), dup.end());

    Fenwick fen(static_cast<int>(sorted.size()));
    long long inversions = 0;
    for (int j = 0; j < n; ++j) {
        int rank = static_cast<int>(std::ranges::lower_bound(sorted, s[j]) -
                                    sorted.begin()) +
                   1;
        inversions += j - fen.prefix(rank);
        fen.add(rank);
    }

    std::println("{}", inversions);
    return 0;
}
