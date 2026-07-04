// cap05/07-quickselect-dutch.cpp
// QuickSelect: k-th element in expected O(n).
// Dutch National Flag: partitions into 3 groups in O(n).
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o quickselect 07-quickselect-dutch.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fequickselect.exe 07-quickselect-dutch.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <random>
#include <vector>

using Vi = std::vector<int>;

// ============================================================
// QuickSelect -- k-th smallest element (0-indexed).
// Randomized partition: expected O(n), worst-case O(n^2).
//
// std::nth_element implements this algorithmic idea.
// The manual version below is for didactic purposes.
// ============================================================
static std::mt19937 rng(42);

// Lomuto partition: places the pivot and returns its final index.
// Guarantees: a[lo..p-1] <= a[p] <= a[p+1..hi] -- O(hi-lo+1)
int partition(Vi& a, int lo, int hi) {
    // Random pivot -- avoids predictable O(n^2) behavior on nearly sorted arrays.
    int r = lo + rng() % (hi - lo + 1);
    std::swap(a[r], a[hi]);

    int pivot = a[hi], wall = lo;
    for (int i = lo; i < hi; ++i)
        if (a[i] <= pivot) std::swap(a[wall++], a[i]);
    std::swap(a[wall], a[hi]);
    return wall;
}

// k-th smallest element (0-indexed). Modifies a in place. Expected O(n).
int quickselect(Vi& a, int lo, int hi, int k) {
    if (lo == hi) return a[lo];
    int p = partition(a, lo, hi);
    if      (k < p) return quickselect(a, lo,    p - 1, k);
    else if (k > p) return quickselect(a, p + 1, hi,    k);
    else            return a[p];
}

// ============================================================
// Dutch National Flag (Dijkstra, 1976)
// Given pivot v, partitions into three groups: < v, == v, > v.
// Used in 3-way QuickSort when many repeated values appear.
// Time: O(n)   Space: O(1)
// ============================================================
void dutch_flag(Vi& a, int pivot) {
    int lo = 0, mid = 0, hi = static_cast<int>(a.size()) - 1;

    // Loop invariant:
    //   a[0..lo-1] < pivot
    //   a[lo..mid-1] == pivot
    //   a[mid..hi] desconhecido
    //   a[hi+1..n-1] > pivot
    while (mid <= hi) {
        if      (a[mid] < pivot) std::swap(a[lo++], a[mid++]);
        else if (a[mid] > pivot) std::swap(a[mid],  a[hi--]);
        else                     ++mid;
    }
}

int main() {
    // QuickSelect manual
    Vi a = {7, 2, 1, 6, 5, 3, 4, 8};
    Vi b = a;
    int k3 = quickselect(b, 0, 7, 2);  // 3rd smallest (0-indexed: index 2) = 3
    assert(k3 == 3);
    std::print("3rd smallest element: {}\n", k3);

    // std::nth_element -- same result, same complexity class.
    Vi c = a;
    std::nth_element(c.begin(), c.begin() + 2, c.end());
    assert(c[2] == 3);
    std::print("std::nth_element[2]: {}\n", c[2]);

    // Dutch National Flag
    Vi d = {2, 0, 1, 2, 1, 0, 2, 0, 1};
    dutch_flag(d, 1);
    // After partitioning: all 0s, then all 1s, then all 2s.
    int lo_end = 0, hi_start = static_cast<int>(d.size());
    while (lo_end < static_cast<int>(d.size()) && d[lo_end] < 1) ++lo_end;
    while (hi_start > 0 && d[hi_start-1] > 1) --hi_start;
    for (int i = 0; i < lo_end;                       ++i) assert(d[i] == 0);
    for (int i = lo_end; i < hi_start;                ++i) assert(d[i] == 1);
    for (int i = hi_start; i < static_cast<int>(d.size()); ++i) assert(d[i] == 2);
    std::print("dutch flag: ");
    for (int x : d) std::print("{} ", x);
    std::print("\n");

    std::print("07-quickselect-dutch: all assertions passed\n");
    return 0;
}
