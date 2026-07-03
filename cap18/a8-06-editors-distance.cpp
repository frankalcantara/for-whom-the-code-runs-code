// Exercise A8.6 - The Editor's Distance
// Session: A8, exercise 6 of 10
// Theme: chapter 08 (edit-distance DP)
// Tags: chapter-08, dynamic-programming
//
// MODEL: minimum insert, delete, or replace operations to transform
//   one string into another.
// MATH: dp[i][j] is the distance between prefixes a[0..i) and
//   b[0..j). Only the previous row is needed.
// PROOF: last-operation induction. The final operation is either
//   insert, delete, replace, or no operation when last characters
//   match; the recurrence takes the best valid predecessor.
// COMPLEXITY: O(|a||b|) time and O(|b|) memory.
// TYPES: distances are at most 2000, so int is ample.
// ALTERNATIVES: full table is simpler for reconstruction, not needed
//   here.
// EDGE CASES: equal strings; length-one strings; all replacements.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o distance a8-06-editors-distance.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string a, b;
    std::cin >> a >> b;
    std::vector<int> prev(b.size() + 1), cur(b.size() + 1);
    for (std::size_t j = 0; j <= b.size(); ++j) prev[j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= a.size(); ++i) {
        cur[0] = static_cast<int>(i);
        for (std::size_t j = 1; j <= b.size(); ++j) {
            if (a[i - 1] == b[j - 1]) cur[j] = prev[j - 1];
            else cur[j] = 1 + std::min({prev[j], cur[j - 1], prev[j - 1]});
        }
        std::swap(prev, cur);
    }
    std::print("{}\n", prev[b.size()]);
    return 0;
}
