// cap05/11-pivot-index.cpp
// Pivot Index: find the index where the sum of all elements to the left
// equals the sum of all elements to the right.
//
// Problem: given an array a[0..n-1], find the smallest index i such that:
//   a[0]+...+a[i-1] == a[i+1]+...+a[n-1]
// If no such index exists, return -1.
//
// This is a direct application of prefix sums: compute the total and
// track left_sum as we scan. At each i, right_sum = total - left_sum - a[i].
//
// Complexity: O(n) time, O(1) extra space.
//
// Source: "O Equilibrista de Pratos" — Frank Alcantara, competitive programming
// course, Phase 1, Week 2 (2024).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o pivot_index 11-pivot-index.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fepivot_index.exe 11-pivot-index.cpp

#include <cassert>
#include <numeric>   // std::accumulate, std::reduce
#include <print>
#include <vector>

// ============================================================
// Brute force: O(n^2) — for every i, compute left and right sums explicitly.
// Used only to verify the O(n) solution on small inputs.
// ============================================================
int pivot_brute(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ++i) {
        long long left  = std::accumulate(a.begin(), a.begin() + i, 0LL);
        long long right = std::accumulate(a.begin() + i + 1, a.end(), 0LL);
        if (left == right) return i;
    }
    return -1;
}

// ============================================================
// Optimal O(n): one pass for total, one pass to track left_sum.
// right_sum = total - left_sum - a[i]  =>  left_sum == right_sum
// when:  2 * left_sum + a[i] == total
// ============================================================
int pivot_optimal(const std::vector<int>& a) {
    long long total     = std::accumulate(a.begin(), a.end(), 0LL);
    long long left_sum  = 0;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        // right_sum = total - left_sum - a[i]
        if (left_sum == total - left_sum - a[i]) return i;
        left_sum += a[i];
    }
    return -1;
}

int main() {
    // Verification: brute force == optimal on all test cases
    struct TestCase { std::vector<int> a; int expected; };
    std::vector<TestCase> tests = {
        {{1, 7, 3, 6, 5, 6},  3},   // left=[1,7,3]=11, right=[5,6]=11
        {{1, 2, 3},          -1},   // no pivot
        {{2, 1, -1},          0},   // left=[], right=[1,-1]=0
        {{0},                 0},   // single element: both sides empty (sum=0)
        {{-1, -1, -1, 0, 1, 1}, 0}, // left=[], right=[-1,-1,0,1,1]=0
        {{0, 0, 0, 0},        0},   // multiple pivots: return smallest
    };

    for (auto& [a, expected] : tests) {
        int r_opt   = pivot_optimal(a);
        int r_brute = pivot_brute(a);
        assert(r_opt == expected);
        assert(r_brute == expected);
        std::print("pivot({}) = {} OK\n", a.size(), r_opt);
    }

    // ============================================================
    // stdin/stdout interface (competition format):
    //   Line 1: n
    //   Line 2: n integers
    //   Output: 0-indexed pivot or -1
    // ============================================================
    // Example run (manual):
    //   6
    //   1 7 3 6 5 6
    //   => 3
    std::print("11-pivot-index: all asserts passed.\n");
    return 0;
}
