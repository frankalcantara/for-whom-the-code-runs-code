// cap05/12-fixed-window-sum.cpp
// Fixed-Length Sliding Window: find the first subarray of length k
// whose sum equals a target value.
//
// Problem (Phase 1, Problem 5): given a 1-indexed sorted array of n integers,
// find a contiguous subarray of length k whose sum equals target.
// Return its 1-based starting index, or -1 if no such subarray exists.
// The solution must use O(1) extra space.
//
// Key insight: compute the sum of the first window in O(k), then slide:
//   add a[i], remove a[i-k].  Each slide is O(1).
//
// Complexity: O(n) time, O(1) extra space.
//
// Source: Phase 1, Problem 5 — competitive programming course (2024).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o fixed_window 12-fixed-window-sum.cpp

#include <cassert>
#include <numeric>   // std::accumulate
#include <print>
#include <vector>

// O(n*k) brute force — for verification on small inputs only.
int window_brute(const std::vector<int>& a, int k, long long target) {
    int n = static_cast<int>(a.size());
    for (int i = 0; i + k <= n; ++i) {
        long long s = std::accumulate(a.begin() + i, a.begin() + i + k, 0LL);
        if (s == target) return i + 1;  // 1-based
    }
    return -1;
}

// O(n) sliding window.
// Initial window: a[0..k-1].  Slide: add a[i], subtract a[i-k].
int fixed_window_sum(const std::vector<int>& a, int k, long long target) {
    int n = static_cast<int>(a.size());
    if (k > n) return -1;
    long long w = std::accumulate(a.begin(), a.begin() + k, 0LL);
    if (w == target) return 1;
    for (int i = k; i < n; ++i) {
        w += a[i] - a[i - k];         // slide one position right
        if (w == target) return i - k + 2;  // 1-based start of window [i-k+1..i]
    }
    return -1;
}

int main() {
    struct TC { std::vector<int> a; int k; long long target; int expected; };
    std::vector<TC> tests = {
        {{1, 2, 3, 4, 5},     2,  5,  2},   // [2,3] starts at index 2
        {{1, 2, 3, 4, 5},     3,  9,  2},   // [2,3,4] starts at index 2
        {{1, 2, 3, 4, 5},     3,  6,  1},   // [1,2,3] starts at index 1
        {{1, 2, 3, 4, 5},     3, 12,  3},   // [3,4,5] starts at index 3
        {{1, 2, 3, 4, 5},     2, 10, -1},   // no window
        {{5},                  1,  5,  1},   // single element
        {{1, 3, 5, 7, 9},     2, 12,  3},   // [5,7] starts at index 3
    };

    for (auto& [a, k, target, expected] : tests) {
        int r_opt   = fixed_window_sum(a, k, target);
        int r_brute = window_brute(a, k, target);
        assert(r_opt == expected);
        assert(r_brute == expected);
        std::print("window(k={}, target={}) = {} OK\n", k, target, r_opt);
    }

    // ============================================================
    // Competition interface (stdin/stdout):
    //   Line 1: n
    //   Line 2: n integers (sorted, 1-indexed per problem, but 0-indexed here)
    //   Line 3: k target
    //   Output: 1-based start index, or -1
    // ============================================================
    std::print("12-fixed-window-sum: all asserts passed.\n");
    return 0;
}
