// cap04/01-binary-search.cpp
// Manual binary search: loop invariant, three canonical forms.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o binary_search 01-binary-search.cpp
// Test:    echo "" | ./binary_search

#include <cassert>
#include <print>
#include <vector>

// ============================================================
// Naive implementation: linear search.
// Baseline for time comparisons in chapter benchmarks.
// Time: O(n)
// ============================================================
int linear_search(const std::vector<int>& v, int target) {
    for (int i = 0; i < static_cast<int>(v.size()); ++i)
        if (v[i] == target) return i;
    return -1;
}

// ============================================================
// Classic error: overflow in mid = (lo + hi) / 2.
// Para lo = 2^30 e hi = 2^30 + 1, lo + hi overflows int.
// ============================================================

// ============================================================
// Form 1: find the first index with v[i] >= target.
// Invariant: v[lo-1] < target and v[hi] >= target, or hi = n.
// Postcondition: lo == hi == result.
// Time: O(log n)
// ============================================================
int lower_bound_manual(const std::vector<int>& v, int target) {
    int lo = 0, hi = static_cast<int>(v.size());   // half-open interval [lo, hi)
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;              // no overflow: mid in [lo, hi)
        if (v[mid] < target) lo = mid + 1;         // target is to the right of mid
        else                 hi = mid;              // mid may be the answer
    }
    return lo;  // lo == hi: first index >= target; == n if none exists
}

// ============================================================
// Form 2: find the last index with v[i] <= target.
// Equivalente a upper_bound(target) - 1
// Invariant: v[lo] <= target and v[hi+1] > target.
// Time: O(log n)
// ============================================================
int upper_bound_manual(const std::vector<int>& v, int target) {
    int lo = 0, hi = static_cast<int>(v.size());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] <= target) lo = mid + 1;        // mid is still <= target, go right
        else                  hi = mid;
    }
    return lo;  // first index > target; lo-1 is the last index <= target
}

// ============================================================
// Form 3: monotone predicate, binary search on answers.
// Finds the smallest x in [lo, hi] such that pred(x) == true.
// Invariant: pred is false for every x < result, true for every x >= result.
// Time: O(log(hi - lo) * custo(pred))
// ============================================================
template <typename Pred>
int first_true(int lo, int hi, Pred pred) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (pred(mid)) hi = mid;    // mid may be the answer; restrict hi
        else           lo = mid + 1; // mid is not the answer; discard it
    }
    return lo;  // pred(lo) == true and pred(lo-1) == false
}

int main() {
    std::vector<int> v = {1, 3, 5, 7, 9, 11, 13, 15};

    // lower_bound: first index >= 7, index 3
    assert(lower_bound_manual(v, 7)  == 3);
    assert(lower_bound_manual(v, 6)  == 3);   // 6 is missing; lands on 7
    assert(lower_bound_manual(v, 16) == 8);   // greater than all elements: returns n

    // upper_bound: first index > 7, index 4
    assert(upper_bound_manual(v, 7)  == 4);
    assert(upper_bound_manual(v, 6)  == 3);   // > 6 starts at index 3
    assert(upper_bound_manual(v, 15) == 8);   // > 15: retorna n

    // Occurrence count: upper - lower
    std::vector<int> dup = {1, 3, 3, 3, 5, 7};
    int cnt = upper_bound_manual(dup, 3) - lower_bound_manual(dup, 3);  // = 3
    assert(cnt == 3);

    // first_true: smallest x in [0,100] such that x*x >= 50; answer is 8.
    int ans = first_true(0, 100, [](int x) { return x * x >= 50; });
    assert(ans == 8);

    std::print("01-binary-search: all assertions passed\n");
    return 0;
}
