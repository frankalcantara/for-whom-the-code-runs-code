// Exercise T08.5 - The Notary's Copies
// Session: T08.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 08 (edit distance; two-row rolling table)
// Tags: chapter-08, dynamic-programming, edit-distance
//
// MODEL: Levenshtein distance between strings a (length n) and b
//   (length m): the minimum number of insertions, deletions and
//   substitutions transforming a into b.
// MATH: dp[i][j] = distance between the prefixes a[0..i) and b[0..j).
//   Base: dp[i][0] = i (delete everything), dp[0][j] = j (insert
//   everything). Step: if a[i-1] == b[j-1], dp[i][j] = dp[i-1][j-1];
//   otherwise 1 + min(dp[i-1][j-1] substitution, dp[i-1][j] deletion,
//   dp[i][j-1] insertion). Row i depends only on row i-1 and the
//   current row's previous entry, so two rows of m+1 ints suffice.
// PROOF: consider an optimal script transforming a[0..i) into b[0..j)
//   and look at its last operation touching the ends: it either
//   matches/substitutes a's last character into b's last, deletes a's
//   last, or inserts b's last. Each case leaves an optimal script for
//   the corresponding smaller pair (cut and paste), so the recurrence's
//   three options are exhaustive and the minimum is correct, by
//   induction on i + j. Scripts never need to edit an edited position
//   twice (collapse the two edits), so this case analysis loses
//   nothing.
// COMPLEXITY: O(nm) time, 2.5e7 cell updates at 5000 x 5000, well
//   within a second; O(m) memory with the rolling rows - 25M ints for
//   the full table would also fit here but scales worse than the habit
//   of rolling.
// TYPES: distances are at most max(n, m) <= 5000: int with absurd
//   slack. Indices in int. The lesson is the table's shape, not its
//   width.
// ALTERNATIVES: LCS gives the answer only when substitutions are
//   banned (distance = n + m - 2 lcs); Hirschberg recovers the actual
//   edit script in O(m) memory (not asked: the court wants the count,
//   not the markup); bit-parallel Myers runs in O(nm / 64) - Chapter
//   12 territory for when 5000 becomes 5e5.
// EDGE CASES: identical copies (0); one copy a prefix of the other
//   (pure insertions); disjoint alphabets (max(n, m)); single
//   characters, equal and different (0 and 1).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o copies t08-s4-1-notarys-copies.cpp

#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string a;
    std::string b;
    std::cin >> a >> b;

    const std::size_t m = b.size();
    std::vector<int> prev(m + 1);
    std::vector<int> curr(m + 1);
    std::iota(prev.begin(), prev.end(), 0);  // dp[0][j] = j

    for (std::size_t i = 1; i <= a.size(); ++i) {
        curr[0] = static_cast<int>(i);  // dp[i][0] = i
        for (std::size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + std::min({prev[j - 1], prev[j], curr[j - 1]});
            }
        }
        std::swap(prev, curr);
    }
    std::print("{}\n", prev[m]);
    return 0;
}
