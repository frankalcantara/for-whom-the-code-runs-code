// cap08/04-matrix-chain.cpp
// Matrix Chain Multiplication and interval DP.
// Palindrome Partitioning: minimum cuts for a palindrome partition.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o matrix_chain 04-matrix-chain.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fematrix_chain.exe 04-matrix-chain.cpp

#include <cassert>
#include <algorithm>
#include <climits>
#include <print>
#include <string>
#include <vector>

using i64 = long long;

// dims[i] = rows of Ai; dims[i+1] = columns of Ai. n matrices: |dims| = n+1.
i64 matrix_chain(const std::vector<int>& dims) {
    int n = static_cast<int>(dims.size()) - 1;
    std::vector<std::vector<i64>> dp(n, std::vector<i64>(n, 0));

    for (int len = 2; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            dp[i][j] = LLONG_MAX;
            for (int k = i; k < j; ++k) {
                i64 cost = dp[i][k] + dp[k+1][j]
                          + (i64)dims[i] * dims[k+1] * dims[j+1];
                dp[i][j] = std::min(dp[i][j], cost);
            }
        }
    return dp[0][n-1];
}

// Palindrome Partitioning: minimum cuts to split s into palindromes.
// is_pal[i][j] in O(n^2), then dp[i] in O(n^2) -- total O(n^2).
int min_palindrome_cuts(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<std::vector<bool>> is_pal(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; ++i) is_pal[i][i] = true;
    for (int i = 0; i < n-1; ++i) is_pal[i][i+1] = (s[i] == s[i+1]);
    for (int len = 3; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            is_pal[i][j] = (s[i] == s[j]) && is_pal[i+1][j-1];
        }

    std::vector<int> dp(n, INT_MAX);
    for (int i = 0; i < n; ++i) {
        if (is_pal[0][i]) { dp[i] = 0; continue; }
        for (int k = 1; k <= i; ++k)
            if (is_pal[k][i])
                dp[i] = std::min(dp[i], dp[k-1] + 1);
    }
    return dp[n-1];
}

int main() {
    // Matrix Chain: A(30x35)*B(35x15)*C(15x5)*D(5x10)*E(10x20)*F(20x25)
    std::vector<int> dims = {30, 35, 15, 5, 10, 20, 25};
    assert(matrix_chain(dims) == 15125);
    std::print("matrix chain cost: {}\n", matrix_chain(dims));

    // 2 matrices (10x30)*(30x5): cost = 10*30*5 = 1500.
    assert(matrix_chain({10, 30, 5}) == 1500);

    // Palindrome cuts
    assert(min_palindrome_cuts("aab")    == 1);  // "aa|b"
    assert(min_palindrome_cuts("a")      == 0);
    assert(min_palindrome_cuts("ab")     == 1);
    // "aaabaa": "a" + "aabaa" (palindrome) -- 1 cut.
    assert(min_palindrome_cuts("aaabaa") == 1);
    std::print("palindrome cuts(aab)    = {}\n", min_palindrome_cuts("aab"));
    std::print("palindrome cuts(aaabaa) = {}\n", min_palindrome_cuts("aaabaa"));

    std::print("04-matrix-chain: all assertions passed\n");
    return 0;
}
