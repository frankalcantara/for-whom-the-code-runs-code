// cap12/01-kmp-z.cpp
// KMP (Knuth-Morris-Pratt) and Z-function for pattern matching in O(n + m).
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o kmp_z 01-kmp-z.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <string>
#include <vector>

std::vector<int> kmp_failure(const std::string& pat) {
    int m = static_cast<int>(pat.size());
    std::vector<int> f(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && pat[i] != pat[j]) j = f[j - 1];
        if (pat[i] == pat[j]) ++j;
        f[i] = j;
    }
    return f;
}

std::vector<int> kmp_search(const std::string& txt, const std::string& pat) {
    if (pat.empty()) return {};
    auto f = kmp_failure(pat);
    int n = static_cast<int>(txt.size());
    int m = static_cast<int>(pat.size());
    std::vector<int> matches;
    for (int i = 0, j = 0; i < n; ++i) {
        while (j > 0 && txt[i] != pat[j]) j = f[j - 1];
        if (txt[i] == pat[j]) ++j;
        if (j == m) {
            matches.push_back(i - m + 1);
            j = f[j - 1];
        }
    }
    return matches;
}

std::vector<int> z_function(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> z(n, 0);
    if (n == 0) return z;
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = std::min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}

std::vector<int> z_search(const std::string& txt, const std::string& pat) {
    if (pat.empty()) return {};
    std::string s = pat + '#' + txt;
    auto z = z_function(s);
    int m = static_cast<int>(pat.size());
    int offset = m + 1;
    std::vector<int> matches;
    for (int i = offset; i < static_cast<int>(s.size()); ++i)
        if (z[i] == m) matches.push_back(i - offset);
    return matches;
}

int main() {
    auto f = kmp_failure("abcabc");
    assert((f == std::vector<int>{0, 0, 0, 1, 2, 3}));

    auto f2 = kmp_failure("aabaaab");
    assert((f2 == std::vector<int>{0, 1, 0, 1, 2, 2, 3}));

    auto m1 = kmp_search("abcabcabc", "abc");
    assert((m1 == std::vector<int>{0, 3, 6}));

    auto m2 = kmp_search("aaaaa", "aaa");
    assert((m2 == std::vector<int>{0, 1, 2}));

    auto m3 = kmp_search("abcdef", "xyz");
    assert(m3.empty());
    assert(kmp_search("abc", "").empty());

    auto z = z_function("aabxaa");
    assert(z[0] == 6);
    assert(z[1] == 1);
    assert(z[4] == 2);

    auto zm1 = z_search("abcabcabc", "abc");
    assert(zm1 == m1);

    auto zm2 = z_search("aaaaa", "aaa");
    assert(zm2 == m2);

    std::print("01-kmp-z: all assertions passed\n");
    return 0;
}
