// cap04/03-sort.cpp
// std::sort, std::stable_sort, comparators, projections, std::ranges::sort.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o sort_demo 03-sort.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesort_demo.exe 03-sort.cpp

#include <algorithm>
#include <cassert>
#include <functional>
#include <print>
#include <ranges>
#include <string>
#include <vector>

// ============================================================
// Example struct for demonstrating comparators and projections.
// ============================================================
struct Contestant {
    int    id;
    int    score;
    std::string name;
};

// ============================================================
// Layer 1: naive bubble sort, O(n^2).
// Used only as a baseline, never in real code.
// ============================================================
void bubble_sort(std::vector<int>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j + 1 < n - i; ++j)
            if (v[j] > v[j + 1]) std::swap(v[j], v[j + 1]);
}

// ============================================================
// Layer 2: std::sort, Introsort, guaranteed O(n log n).
// ============================================================

void demo_sort_basic() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3};

    // Ascending, the default.
    std::ranges::sort(v);
    assert(std::ranges::is_sorted(v));

    // Descendente
    std::ranges::sort(v, std::greater<>{});
    assert(std::ranges::is_sorted(v, std::greater<>{}));
}

// ============================================================
// Custom comparator: sort by absolute value.
// ============================================================
void demo_custom_comparator() {
    std::vector<int> v = {-5, 3, -1, 4, -2};

    std::ranges::sort(v, [](int a, int b) {
        return std::abs(a) < std::abs(b);   // O(n log n)
    });
    // Result: {-1, -2, 3, 4, -5}
    assert(v[0] == -1 && v[4] == -5);
    std::print("sort by |x|: ");
    for (int x : v) std::print("{} ", x);
    std::print("\n");
}

// ============================================================
// Projection: sort by a struct field without an explicit comparator.
// std::ranges::sort(v, cmp, proj): proj is applied before cmp.
// ============================================================
void demo_projections() {
    std::vector<Contestant> cs = {
        {1, 80, "Carlos"},
        {2, 95, "Alice"},
        {3, 80, "Bruno"},
        {4, 72, "Diana"},
    };

    // Sort by decreasing score; on ties, by ascending name.
    // Two levels: first stable sort by name, then stable sort by score.
    std::ranges::stable_sort(cs, {}, &Contestant::name);  // O(n log n)
    std::ranges::stable_sort(cs, std::greater<>{}, &Contestant::score); // O(n log n)

    // Result: Alice(95), Bruno(80), Carlos(80), Diana(72).
    assert(cs[0].name == "Alice");
    assert(cs[1].name == "Bruno");   // Bruno before Carlos, lexicographic order
    assert(cs[2].name == "Carlos");

    std::print("ranking: ");
    for (auto& c : cs)
        std::print("{}({}) ", c.name, c.score);
    std::print("\n");
}

// ============================================================
// std::stable_sort: preserves relative order of equivalent elements.
// Usually implemented with merge sort, O(n log n), O(n) extra space.
// ============================================================
void demo_stable_sort() {
    // Offline queries: sort by right endpoint while preserving original order
    // among queries with the same endpoint.
    struct Query { int l, r, idx; };
    std::vector<Query> qs = {{0,5,0}, {2,5,1}, {1,3,2}, {0,3,3}};

    std::ranges::stable_sort(qs, {}, &Query::r);  // stable sort by field r
    // Queries with r=3: idx=2 before idx=3, original order preserved.
    assert(qs[0].idx == 2);
    assert(qs[1].idx == 3);
}

// ============================================================
// STL lower_bound and upper_bound after sorting.
// ============================================================
void demo_binary_search_stl() {
    std::vector<int> v = {1, 3, 3, 3, 5, 7, 9};

    // Count occurrences of 3: O(log n).
    auto lo = std::ranges::lower_bound(v, 3);  // first index >= 3
    auto hi = std::ranges::upper_bound(v, 3);  // first index > 3
    int cnt = static_cast<int>(hi - lo);       // = 3
    assert(cnt == 3);

    // Check existence: O(log n).
    bool found = std::ranges::binary_search(v, 5);
    assert(found);
    assert(!std::ranges::binary_search(v, 4));

    // Projection: binary_search in a vector of structs by score field.
    std::vector<Contestant> cs = {{1,70,"A"},{2,80,"B"},{3,95,"C"}};
    // cs is already sorted by score.
    bool has95 = std::ranges::binary_search(cs, 95, {}, &Contestant::score);
    assert(has95);
}

int main() {
    demo_sort_basic();
    demo_custom_comparator();
    demo_projections();
    demo_stable_sort();
    demo_binary_search_stl();
    std::print("03-sort: all assertions passed\n");
    return 0;
}
