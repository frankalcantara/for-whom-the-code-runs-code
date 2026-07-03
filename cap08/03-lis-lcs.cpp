// cap08/03-lis-lcs.cpp
// LIS (Longest Increasing Subsequence), LCS, and Edit Distance.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o lis_lcs 03-lis-lcs.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using Vi = std::vector<int>;

// ============================================================
// LIS O(n^2): dp[i] = length of the LIS ending at a[i].
// dp[i] = max(dp[j] + 1) para j < i com a[j] < a[i]
// ============================================================
int lis_n2(const Vi& a) {
    int n = static_cast<int>(a.size());
    if (n == 0) return 0;
    Vi dp(n, 1);
    for (int i = 1; i < n; ++i)
        for (int j = 0; j < i; ++j)
            if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
    return std::ranges::max(dp);
}

// ============================================================
// LIS O(n log n): patience sorting (Robinson-Schensted-Knuth)
// tails[k] = smallest final value of an increasing subsequence of length k+1.
// Invariant: tails is always sorted -- lower_bound in O(log n).
// ============================================================
int lis_nlogn(const Vi& a) {
    Vi tails;
    for (int x : a) {
        auto it = std::ranges::lower_bound(tails, x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return static_cast<int>(tails.size());
}

// ============================================================
// LCS (Longest Common Subsequence)
// dp[i][j] = LCS of a[0..i) and b[0..j).
// Rolling array: O(min(m,n)) space.
// ============================================================
int lcs(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());
    std::vector<int> dp(n + 1, 0), prev(n + 1, 0);

    for (int i = 1; i <= m; ++i) {
        std::swap(dp, prev);
        dp.assign(n + 1, 0);
        for (int j = 1; j <= n; ++j) {
            if (a[i-1] == b[j-1]) dp[j] = prev[j-1] + 1;
            else                   dp[j] = std::max(prev[j], dp[j-1]);
        }
    }
    return dp[n];
}

// ============================================================
// Edit Distance (Levenshtein): insertion, deletion, substitution -- cost 1 each.
// dp[i][j] = min operations to convert a[0..i) into b[0..j).
// Rolling array: O(n) space.
// ============================================================
int edit_distance(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());
    std::vector<int> dp(n + 1), prev(n + 1);

    for (int j = 0; j <= n; ++j) prev[j] = j;  // base: convert "" into b[0..j)

    for (int i = 1; i <= m; ++i) {
        dp[0] = i;  // convert a[0..i) into ""
        for (int j = 1; j <= n; ++j) {
            if (a[i-1] == b[j-1]) dp[j] = prev[j-1];
            else dp[j] = 1 + std::min({prev[j-1], prev[j], dp[j-1]});
        }
        std::swap(dp, prev);
    }
    return prev[n];
}

int main() {
    // LIS
    Vi a = {10, 9, 2, 5, 3, 7, 101, 18};
    assert(lis_n2(a)    == 4);  // [2,3,7,18] or [2,5,7,18], etc.
    assert(lis_nlogn(a) == 4);
    std::print("LIS({}) = {}\n", a.size(), lis_nlogn(a));

    Vi b = {0, 1, 0, 3, 2, 3};
    assert(lis_nlogn(b) == 4);  // [0,1,2,3]; strict increase rejects [0,1,3,3].

    // LCS
    assert(lcs("abcde", "ace")  == 3);  // "ace"
    assert(lcs("abc",   "abc")  == 3);
    assert(lcs("abc",   "def")  == 0);
    std::print("LCS(abcde, ace) = {}\n", lcs("abcde", "ace"));

    // Edit Distance
    assert(edit_distance("horse", "ros")     == 3);
    assert(edit_distance("intention", "execution") == 5);
    assert(edit_distance("",    "abc")       == 3);
    assert(edit_distance("abc", "")          == 3);
    std::print("edit(horse, ros) = {}\n", edit_distance("horse", "ros"));
    std::print("edit(intention, execution) = {}\n", edit_distance("intention", "execution"));

    std::print("03-lis-lcs: all assertions passed\n");
    return 0;
}
