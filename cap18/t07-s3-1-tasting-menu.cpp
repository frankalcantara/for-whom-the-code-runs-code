// Exercise T07.3 - The Tasting Menu
// Session: T07.S3, exercise 1 of 2 (1 hour for both)
//
// Theme: chapter 07 (exhaustive enumeration of permutations)
// Tags: chapter-07, sorting, brute-force
//
// MODEL: count permutations of n labeled items subject to an adjacency
//   constraint on a label attribute (the category). With n <= 8 the
//   space has at most 8! = 40320 orders: small enough to enumerate
//   every one and test the constraint directly.
// MATH: the dishes are distinct, so the universe is the n! orderings of
//   indices 0..n-1. An ordering is valid iff for every consecutive pair
//   (p_i, p_{i+1}) the categories differ. std::next_permutation
//   enumerates all n! orderings of a sorted distinct sequence exactly
//   once, in lexicographic order.
// PROOF: correctness is by exhaustion - the loop visits each ordering
//   exactly once (next_permutation's contract on a strictly increasing
//   start) and applies the definition of validity verbatim. For the
//   example AABC: 4! = 24 total; orders where the two A-dishes touch =
//   2 * 3! = 12 (glue them into a block, two internal orders); valid =
//   24 - 12 = 12, matching the program's count.
// COMPLEXITY: O(n! * n) time, O(n) memory; at n = 8 about 3e5 character
//   comparisons. The exercise calibrates a contest instinct: when the
//   bound says 8, the intended solution is often honest brute force,
//   and inventing something cleverer is a way to lose half an hour.
// TYPES: the count is at most 8! = 40320, so int suffices with room to
//   spare; indices are int. Nothing overflows; the discipline is in the
//   enumeration, not the arithmetic.
// ALTERNATIVES: inclusion-exclusion over category blocks gives a
//   closed-form count without enumeration (Chapter 11 territory, not
//   yet available to the student at T07); recursive backtracking with
//   early pruning on the last-served category is the same count with
//   less wasted work - worthwhile only when n grows past what n! allows.
// EDGE CASES: n = 1 (answer 1, no adjacent pair exists); all dishes the
//   same category with n > 1 (answer 0); all categories distinct
//   (answer n!); two dishes, same category (0) vs different (2).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o menu t07-s3-1-tasting-menu.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femenu.exe t07-s3-1-tasting-menu.cpp

#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::string category;
    std::cin >> category;

    std::vector<int> order(static_cast<std::size_t>(n));
    std::iota(order.begin(), order.end(), 0);

    long long valid = 0;
    do {
        bool ok = true;
        for (int i = 0; i + 1 < n && ok; ++i) {
            ok = category[static_cast<std::size_t>(order[static_cast<std::size_t>(i)])] !=
                 category[static_cast<std::size_t>(order[static_cast<std::size_t>(i) + 1])];
        }
        valid += ok ? 1 : 0;
    } while (std::next_permutation(order.begin(), order.end()));

    std::print("{}\n", valid);
    return 0;
}
