// cap05/03-sliding-window.cpp
// Sliding window with deque -- minimum and maximum over all windows in O(n).
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o sliding_window 03-sliding-window.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesliding_window.exe 03-sliding-window.cpp

#include <cassert>
#include <deque>
#include <print>
#include <vector>

using Vi = std::vector<int>;

// ============================================================
// Sliding-window minimum for windows of size k.
// Returns a vector of size n-k+1 with the minimum of each window.
//
// Deque invariant: stores indices of a[] in increasing order of a[i].
// The deque front is always the index of the current window minimum.
// Time: O(n) -- each element enters and leaves the deque at most once.
// ============================================================
Vi sliding_min(const Vi& a, int k) {
    int n = static_cast<int>(a.size());
    Vi result;
    result.reserve(n - k + 1);
    std::deque<int> dq;  // stores indices, not values

    for (int i = 0; i < n; ++i) {
        // Remove indices outside the current window [i-k+1, i].
        while (!dq.empty() && dq.front() < i - k + 1)
            dq.pop_front();

        // Remove indices whose values are >= a[i]; they cannot be future minima.
        // Keeps the deque in increasing order of a[j].
        while (!dq.empty() && a[dq.back()] >= a[i])
            dq.pop_back();

        dq.push_back(i);

        // The first complete window appears when i == k-1.
        if (i >= k - 1)
            result.push_back(a[dq.front()]);
    }
    return result;
}

// ============================================================
// Sliding-window maximum -- same idea, deque in decreasing value order.
// Time: O(n)
// ============================================================
Vi sliding_max(const Vi& a, int k) {
    int n = static_cast<int>(a.size());
    Vi result;
    result.reserve(n - k + 1);
    std::deque<int> dq;

    for (int i = 0; i < n; ++i) {
        while (!dq.empty() && dq.front() < i - k + 1)
            dq.pop_front();
        while (!dq.empty() && a[dq.back()] <= a[i])
            dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1)
            result.push_back(a[dq.front()]);
    }
    return result;
}

// ============================================================
// Variable-size window with sum <= target.
// Finds the longest subarray with sum <= target using two pointers.
// Time: O(n) for non-negative integers.
// ============================================================
int max_len_sum_leq(const Vi& a, long long target) {
    int lo = 0, best = 0;
    long long sum = 0;
    for (int hi = 0; hi < static_cast<int>(a.size()); ++hi) {
        sum += a[hi];
        while (sum > target) sum -= a[lo++];
        best = std::max(best, hi - lo + 1);
    }
    return best;
}

int main() {
    Vi a = {1, 3, -1, -3, 5, 3, 6, 7};

    Vi mn = sliding_min(a, 3);
    // Windows: {1,3,-1}->-1, {3,-1,-3}->-3, {-1,-3,5}->-3,
    // {-3,5,3}->-3, {5,3,6}->3, {3,6,7}->3.
    assert((mn == Vi{-1, -3, -3, -3, 3, 3}));

    Vi mx = sliding_max(a, 3);
    // Window maxima: 3, 3, 5, 5, 6, 7.
    assert((mx == Vi{3, 3, 5, 5, 6, 7}));

    std::print("sliding min(k=3): ");
    for (int x : mn) std::print("{} ", x);
    std::print("\n");

    std::print("sliding max(k=3): ");
    for (int x : mx) std::print("{} ", x);
    std::print("\n");

    Vi b = {1, 2, 3, 4, 5};
    int len = max_len_sum_leq(b, 9);  // {1,2,3,4} sums to 10 > 9; {2,3,4}=9.
    assert(len == 3);
    std::print("longest subarray with sum <= 9: length = {}\n", len);

    std::print("03-sliding-window: all assertions passed\n");
    return 0;
}
