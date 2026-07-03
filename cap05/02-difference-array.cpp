// cap05/02-difference-array.cpp
// Difference array -- range updates in O(1), reconstruction in O(n).
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o diff_array 02-difference-array.cpp

#include <cassert>
#include <numeric>
#include <print>
#include <vector>

using Vi = std::vector<int>;

// ============================================================
// Difference Array
// update(l, r, v): adds v to all elements of a[l..r] -- O(1)
// build(): reconstructs the array after all updates -- O(n)
//
// Invariante: d[i] = a[i] - a[i-1] (com a[-1] = 0)
// A +v update on [l,r] becomes d[l] += v and d[r+1] -= v.
// Prefixing d reconstructs a.
// ============================================================
struct DiffArray {
    Vi d;
    int n;

    explicit DiffArray(int n) : d(n + 1, 0), n(n) {}

    // Initializes from an existing array.
    explicit DiffArray(const Vi& a)
        : d(a.size() + 1, 0), n(static_cast<int>(a.size()))
    {
        for (int i = 0; i < n; ++i) {
            d[i]   += a[i];
            d[i+1] -= a[i];
        }
    }

    // Adds v to a[l..r] (0-indexed, closed). O(1).
    void update(int l, int r, int v) {
        d[l]    += v;
        d[r + 1] -= v;
    }

    // Returns the array after all updates. O(n).
    Vi build() const {
        Vi a(n);
        int running = 0;
        for (int i = 0; i < n; ++i) {
            running += d[i];
            a[i] = running;
        }
        return a;
    }
};

int main() {
    // Array inicial: [0, 0, 0, 0, 0]
    DiffArray da(5);
    da.update(1, 3, 2);   // [0, 2, 2, 2, 0]
    da.update(0, 2, 1);   // [1, 3, 3, 2, 0]
    da.update(2, 4, -1);  // [1, 3, 2, 1, -1]

    Vi result = da.build();
    assert((result == Vi{1, 3, 2, 1, -1}));

    std::print("diff array: ");
    for (int x : result) std::print("{} ", x);
    std::print("\n");

    // From an existing array.
    DiffArray da2(Vi{5, 3, 7, 2, 9});
    da2.update(0, 4, 10);  // +10 to all positions
    da2.update(1, 2, -5);  // -5 on [1,2]
    Vi r2 = da2.build();   // {15, 8, 12, 12, 19}
    assert(r2[0] == 15 && r2[1] == 8 && r2[2] == 12);

    std::print("02-difference-array: all assertions passed\n");
    return 0;
}
