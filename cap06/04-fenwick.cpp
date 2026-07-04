// cap06/04-fenwick.cpp
// Fenwick Tree (Binary Indexed Tree) -- point update + range query.
// Variants: difference-style range update, 2D, and order-statistics patterns.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o fenwick 04-fenwick.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fefenwick.exe 04-fenwick.cpp

#include <cassert>
#include <print>
#include <vector>

using i64 = long long;
using Vi64 = std::vector<i64>;

// ============================================================
// Fenwick Tree 1D
// Implicitly represents an array of n elements.
// O(log n) per update and prefix query.
//
// Idea: bit[i] stores the sum of an interval whose length is
// given by the least significant set bit: lowbit(i) = i & (-i).
// ============================================================
struct Fenwick {
    int n;
    Vi64 bit;

    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}

    // Sum of a[1..i] (1-indexed): O(log n)
    i64 query(int i) const {
        i64 s = 0;
        for (; i > 0; i -= i & (-i)) s += bit[i];
        return s;
    }

    // Sum of a[l..r] (1-indexed): O(log n)
    i64 query(int l, int r) const { return query(r) - query(l - 1); }

    // Adds v to a[i] (1-indexed): O(log n)
    void update(int i, i64 v) {
        for (; i <= n; i += i & (-i)) bit[i] += v;
    }

    // Initializes from a 0-indexed vector: O(n log n)
    void build(const std::vector<int>& a) {
        for (int i = 0; i < static_cast<int>(a.size()); ++i)
            update(i + 1, a[i]);
    }
};

// ============================================================
// Fenwick with range update + range query.
// Uses two BITs: B1 and B2.
// To add v to a[l..r]:
//   B1[l] += v;  B1[r+1] -= v;
//   B2[l] += v*(l-1);  B2[r+1] -= v*r;
// Query prefix sum a[1..i] = B1.query(i)*i - B2.query(i)
// ============================================================
struct FenwickRangeUpdate {
    int n;
    Fenwick b1, b2;

    explicit FenwickRangeUpdate(int n) : n(n), b1(n), b2(n) {}

    void update(int l, int r, i64 v) {
        b1.update(l, v);      b1.update(r + 1, -v);
        b2.update(l, v*(l-1)); b2.update(r + 1, -v*r);
    }

    i64 query(int i) const {
        return b1.query(i) * i - b2.query(i);
    }

    i64 query(int l, int r) const { return query(r) - query(l - 1); }
};

// ============================================================
// Fenwick 2D: point update + rectangle sum.
// O(log(n) * log(m)) per operation.
// ============================================================
struct Fenwick2D {
    int rows, cols;
    std::vector<Vi64> bit;

    Fenwick2D(int r, int c) : rows(r), cols(c), bit(r + 1, Vi64(c + 1, 0)) {}

    void update(int r, int c, i64 v) {
        for (int i = r; i <= rows; i += i & (-i))
            for (int j = c; j <= cols; j += j & (-j))
                bit[i][j] += v;
    }

    i64 query(int r, int c) const {
        i64 s = 0;
        for (int i = r; i > 0; i -= i & (-i))
            for (int j = c; j > 0; j -= j & (-j))
                s += bit[i][j];
        return s;
    }

    // Sum of rectangle [r1,c1]-[r2,c2] (1-indexed).
    i64 query(int r1, int c1, int r2, int c2) const {
        return query(r2, c2) - query(r1 - 1, c2)
             - query(r2, c1 - 1) + query(r1 - 1, c1 - 1);
    }
};

int main() {
    // Fenwick 1D
    Fenwick fw(8);
    fw.build({3, 1, 4, 1, 5, 9, 2, 6});

    assert(fw.query(1, 8) == 31);
    assert(fw.query(3, 5) == 4 + 1 + 5);
    fw.update(3, 10);  // a[3] += 10: was 4, now 14
    assert(fw.query(3, 5) == 14 + 1 + 5);
    std::print("Fenwick 1D: query(3,5) after update = {}\n", fw.query(3, 5));

    // Fenwick range update
    FenwickRangeUpdate fru(8);
    fru.update(2, 5, 3);  // a[2..5] += 3
    assert(fru.query(1, 8) == 3 * 4);  // 4 updated elements
    assert(fru.query(3, 4) == 6);       // 3+3
    std::print("FenwickRangeUpdate: query(3,4) = {}\n", fru.query(3, 4));

    // Fenwick 2D
    Fenwick2D fw2(3, 3);
    fw2.update(1, 1, 1); fw2.update(2, 2, 5); fw2.update(3, 3, 9);
    assert(fw2.query(1, 1, 3, 3) == 15);
    assert(fw2.query(1, 1, 2, 2) == 6);
    std::print("Fenwick2D: query(1,1,3,3) = {}\n", fw2.query(1, 1, 3, 3));

    std::print("04-fenwick: all assertions passed\n");
    return 0;
}
