// cap07/04-meet-in-middle.cpp
// Meet in the Middle: splits the search space in two, solves each half,
// and combines results with binary search.
//
// Problem: subset sum -- does some subset of a[] sum exactly to T?
//   Naive: O(2^n) -- enumerates every subset.
//   MitM:  O(2^(n/2) * n) -- enumerates each half and combines with binary search.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o mitm 04-meet-in-middle.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femitm.exe 04-meet-in-middle.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <ranges>
#include <vector>

using i64 = long long;

// ============================================================
// Layer 1 -- Naive: O(2^n).
// Enumerates all 2^n subsets with a bitmask.
// ============================================================
bool naive_subset_sum(const std::vector<int>& a, i64 target) {
    int n = static_cast<int>(a.size());
    for (int mask = 0; mask < (1 << n); ++mask) {
        i64 s = 0;
        for (int i = 0; i < n; ++i)
            if (mask >> i & 1) s += a[i];
        if (s == target) return true;
    }
    return false;
}

// ============================================================
// Layer 2 -- Meet in the Middle: O(2^(n/2) * n/2 * log)
// Splits a[] into two halves L and R.
// Generates all subset sums of L and R.
// For each sum of R, uses lower_bound to find the complement in L.
// ============================================================

// Generates all possible subset sums of a[lo..hi).
std::vector<i64> subset_sums(const std::vector<int>& a, int lo, int hi) {
    int sz = hi - lo;
    std::vector<i64> sums;
    sums.reserve(1 << sz);
    for (int mask = 0; mask < (1 << sz); ++mask) {
        i64 s = 0;
        for (int i = 0; i < sz; ++i)
            if (mask >> i & 1) s += a[lo + i];
        sums.push_back(s);
    }
    return sums;
}

bool mitm_subset_sum(const std::vector<int>& a, i64 target) {
    int n = static_cast<int>(a.size());
    int half = n / 2;

    auto left  = subset_sums(a, 0,    half);
    auto right = subset_sums(a, half, n);

    std::ranges::sort(left);

    for (i64 r : right) {
        i64 need = target - r;
        auto it = std::ranges::lower_bound(left, need);
        if (it != left.end() && *it == need) return true;
    }
    return false;
}

// ============================================================
// Variant: counts subsets with sum = target.
// ============================================================
i64 mitm_count(const std::vector<int>& a, i64 target) {
    int n = static_cast<int>(a.size());
    int half = n / 2;

    auto left  = subset_sums(a, 0,    half);
    auto right = subset_sums(a, half, n);

    std::ranges::sort(left);

    i64 cnt = 0;
    for (i64 r : right) {
        i64 need = target - r;
        auto lo = std::ranges::lower_bound(left, need);
        auto hi = std::ranges::upper_bound(left, need);
        cnt += hi - lo;
    }
    return cnt;
}

int main() {
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6};

    // Subset sum: is there a subset with sum 15?
    // 3+1+5+6=15: yes
    assert(naive_subset_sum(a, 15) == true);
    assert(mitm_subset_sum(a, 15)  == true);
    assert(naive_subset_sum(a, 31) == true);   // total sum
    assert(mitm_subset_sum(a, 31)  == true);
    assert(!naive_subset_sum(a, 32));  // does not exist
    assert(!mitm_subset_sum(a, 32));
    std::print("subset_sum(15): {}\n", mitm_subset_sum(a, 15) ? "yes" : "no");

    // Counting: how many subsets sum to 10?
    i64 cnt_naive = 0;
    for (int mask = 0; mask < (1 << 8); ++mask) {
        i64 s = 0;
        for (int i = 0; i < 8; ++i) if (mask>>i&1) s += a[i];
        if (s == 10) ++cnt_naive;
    }
    i64 cnt_mitm = mitm_count(a, 10);
    assert(cnt_naive == cnt_mitm);
    std::print("subset count with sum 10: {} (naive) = {} (mitm)\n",
        cnt_naive, cnt_mitm);

    // n=40 with MitM: 2^20 sums per side, feasible.
    // This example uses n=20 to keep the sample program quick.
    std::vector<int> big(20);
    for (int i = 0; i < 20; ++i) big[i] = i + 1;  // 1..20, sum = 210
    assert(mitm_subset_sum(big, 210) == true);   // total sum
    assert(mitm_subset_sum(big, 100) == true);   // 1+2+...+13+...
    std::print("mitm n=20: ok\n");

    std::print("04-meet-in-middle: all assertions passed\n");
    return 0;
}
