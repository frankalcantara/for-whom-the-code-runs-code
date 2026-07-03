// cap12/02-suffix-array.cpp
// Suffix array with O(n log n) prefix doubling, plus Kasai LCP in O(n).
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o suffix_array 02-suffix-array.cpp

#include <algorithm>
#include <cassert>
#include <numeric>
#include <print>
#include <string>
#include <vector>

std::vector<int> build_suffix_array(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return {};
    std::vector<int> sa(n), rank_(n), tmp(n);

    std::iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) rank_[i] = static_cast<unsigned char>(s[i]);

    for (int gap = 1; gap < n; gap <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
            int ra = (a + gap < n) ? rank_[a + gap] : -1;
            int rb = (b + gap < n) ? rank_[b + gap] : -1;
            return ra < rb;
        };

        std::sort(sa.begin(), sa.end(), cmp);

        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
            tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        rank_ = tmp;
        if (rank_[sa[n - 1]] == n - 1) break;
    }
    return sa;
}

std::vector<int> build_lcp(const std::string& s, const std::vector<int>& sa) {
    int n = static_cast<int>(s.size());
    if (n == 0) return {};
    std::vector<int> rank_(n), lcp(n, 0);
    for (int i = 0; i < n; ++i) rank_[sa[i]] = i;

    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank_[i] == 0) continue;
        int j = sa[rank_[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;
        lcp[rank_[i]] = h;
        if (h > 0) --h;
    }
    return lcp;
}

long long count_distinct_substrings(const std::string& s) {
    auto sa = build_suffix_array(s);
    auto lcp = build_lcp(s, sa);
    int n = static_cast<int>(s.size());
    long long total = 1LL * n * (n + 1) / 2;
    for (int x : lcp) total -= x;
    return total;
}

int main() {
    auto sa = build_suffix_array("banana");
    assert((sa == std::vector<int>{5, 3, 1, 0, 4, 2}));

    auto lcp = build_lcp("banana", sa);
    assert(lcp[0] == 0);
    assert(lcp[1] == 1);
    assert(lcp[2] == 3);
    assert(lcp[3] == 0);
    assert(lcp[5] == 2);

    assert(count_distinct_substrings("banana") == 15);
    assert(count_distinct_substrings("aaaa") == 4);
    assert(count_distinct_substrings("abcd") == 10);
    assert(count_distinct_substrings("") == 0);

    std::print("02-suffix-array: all assertions passed\n");
    return 0;
}
