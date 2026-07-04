// cap05/05-kadane-two-pointers.cpp
// Kadane's Algorithm: maximum contiguous subarray -- O(n)
// Two Pointers: subarray with sum equal to target -- O(n)
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o kadane_tp 05-kadane-two-pointers.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fekadane_tp.exe 05-kadane-two-pointers.cpp

#include <algorithm>
#include <cassert>
#include <climits>
#include <print>
#include <utility>
#include <vector>

using Vi = std::vector<int>;

// ============================================================
// Kadane's algorithm -- maximum-sum subarray.
// Idea: keep the sum of the best subarray ending at i.
// If starting at i is better than extending, restart at i.
//
// Returns {maximum sum, left index, right index} of the optimal subarray.
// Time: O(n)   Space: O(1)
// ============================================================
struct KadaneResult { long long sum; int l, r; };

KadaneResult kadane(const Vi& a) {
    long long best = LLONG_MIN, cur = 0;
    int best_l = 0, best_r = 0, cur_l = 0;

    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        if (cur + a[i] < a[i]) {       // restart: starting at i is better
            cur = a[i];
            cur_l = i;
        } else {
            cur += a[i];
        }
        if (cur > best) {
            best = cur;
            best_l = cur_l;
            best_r = i;
        }
    }
    return {best, best_l, best_r};
}

// ============================================================
// Two pointers -- finds a subarray with sum == target.
// Works only for non-negative integers (monotone invariant).
// Returns {l, r} of the first subarray found, or {-1,-1} if none exists.
// Time: O(n)
// ============================================================
std::pair<int,int> two_pointers_sum(const Vi& a, long long target) {
    int lo = 0;
    long long sum = 0;
    for (int hi = 0; hi < static_cast<int>(a.size()); ++hi) {
        sum += a[hi];
        while (sum > target && lo <= hi) sum -= a[lo++];
        if (sum == target) return {lo, hi};
    }
    return {-1, -1};
}

// ============================================================
// Two pointers -- counts pairs (i,j) with i<j and a[i]+a[j] == target.
// Requires a sorted array. Pointer lo starts at the beginning, hi at the end.
// Time: O(n) after O(n log n) sorting.
// ============================================================
int count_pairs_sum(Vi a, int target) {
    std::ranges::sort(a);
    int lo = 0, hi = static_cast<int>(a.size()) - 1, cnt = 0;
    while (lo < hi) {
        int s = a[lo] + a[hi];
        if      (s < target) ++lo;
        else if (s > target) --hi;
        else {
            if (a[lo] == a[hi]) {        // all remaining elements form pairs
                long long d = hi - lo + 1;
                cnt += static_cast<int>(d * (d - 1) / 2);
                break;
            }
            int cl = 1, cr = 1;
            while (a[lo + 1] == a[lo]) { ++lo; ++cl; }
            while (a[hi - 1] == a[hi]) { --hi; ++cr; }
            cnt += cl * cr;
            ++lo; --hi;
        }
    }
    return cnt;
}

int main() {
    // Kadane
    Vi a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    auto [sum, l, r] = kadane(a);
    assert(sum == 6);    // a[3..6] = 4 + (-1) + 2 + 1 = 6
    assert(l == 3 && r == 6);
    std::print("kadane: sum={} a[{}..{}]\n", sum, l, r);

    // All negative
    Vi neg = {-3, -1, -2};
    auto [s2, l2, r2] = kadane(neg);
    assert(s2 == -1 && l2 == 1 && r2 == 1);

    // Two pointers -- subarray with sum == target
    Vi b = {1, 4, 2, 9, 3, 6};
    auto [pl, pr] = two_pointers_sum(b, 12);
    assert(pl != -1);
    long long s = 0; for (int i = pl; i <= pr; ++i) s += b[i];
    assert(s == 12);
    std::print("two_pointers_sum(12): a[{}..{}] sum={}\n", pl, pr, s);

    // Two pointers -- counts pairs with sum == target
    Vi c = {1, 5, 7, -1, 5};
    int pairs = count_pairs_sum(c, 6);
    assert(pairs == 3);  // (1,5), (1,5), (7,-1)
    std::print("pairs with sum 6: {}\n", pairs);

    std::print("05-kadane-two-pointers: all assertions passed\n");
    return 0;
}
