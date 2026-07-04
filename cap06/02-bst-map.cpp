// cap06/02-bst-map.cpp
// std::set, std::map, std::flat_map (C++23) -- ordered containers and cache-friendly variants.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o bst_map 02-bst-map.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febst_map.exe 02-bst-map.cpp

#include <algorithm>
#include <cassert>
#include <map>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <vector>

// std::flat_map/flat_set: C++23; standard-library support varies
// Commented out for compatibility with older standard libraries; enable in a complete C++23 environment.
// #include <flat_map>

using Vi = std::vector<int>;

// ============================================================
// std::set: ordered tree with search, insertion, and removal in O(log n).
// Use cases: ordered set, predecessor/successor, order-related queries.
// ============================================================
void demo_set() {
    std::set<int> s;
    for (int x : {5, 3, 8, 1, 4, 7, 9}) s.insert(x);

    // lower_bound: first element >= k
    auto it = s.lower_bound(4);
    assert(*it == 4);

    // upper_bound: first element > k
    it = s.upper_bound(4);
    assert(*it == 5);

    // Predecessor of 5: greatest element < 5
    it = s.lower_bound(5);
    --it;
    assert(*it == 4);

    s.erase(4);
    assert(s.count(4) == 0);
    std::print("set: lower/upper bound ok\n");
}

// ============================================================
// std::map: key-value association in O(log n).
// Word frequency, occurrence counting, ordered traversal.
// ============================================================
void demo_map() {
    std::map<std::string, int> freq;
    for (const auto& w : {"ab","cd","ab","ef","cd","ab"})
        ++freq[w];

    assert(freq["ab"] == 3);
    assert(freq["cd"] == 2);
    assert(freq["ef"] == 1);

    // Lexicographic iteration (ordered-tree property).
    std::string prev = "";
    for (const auto& [k, v] : freq) {
        assert(k > prev);
        prev = k;
    }
    std::print("map: frequency ok\n");
}

// ============================================================
// Classic problem: count pairs with difference <= k.
// Uses sorted order for logarithmic/binary-search style counting.
// Naive O(n^2) vs set O(n log n)
// ============================================================
int count_pairs_diff_leq(Vi a, int k) {
    std::ranges::sort(a);
    int cnt = 0;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        // Counts elements in (a[i], a[i]+k] with index > i.
        auto suffix = std::ranges::subrange(a.begin() + i + 1, a.end());
        auto lo = std::ranges::upper_bound(suffix, a[i]);
        auto hi = std::ranges::upper_bound(suffix, a[i] + k);
        cnt += static_cast<int>(hi - lo);
    }
    return cnt;
}

// ============================================================
// std::flat_map (C++23): sorted contiguous key-value storage.
// Cache-friendly for read-heavy tables; insertion is O(n).
// Availability depends on the standard library implementation.
// ============================================================
// void demo_flat_map() {
//     std::flat_map<int,int> fm;
//     fm[1] = 10; fm[3] = 30; fm[2] = 20;
//     assert(fm[2] == 20);
//     // Typically stores keys and values contiguously.
//     // Iteration is contiguous; insertion may shift elements.
//     std::print("flat_map: ok\n");
// }

int main() {
    demo_set();
    demo_map();

    // count_pairs_diff_leq
    assert(count_pairs_diff_leq({1,3,5,7}, 2) == 3);  // (1,3),(3,5),(5,7)
    assert(count_pairs_diff_leq({1,2,3,4}, 0) == 0);
    std::print("count_pairs_diff_leq: {}\n", count_pairs_diff_leq({1,3,5,7}, 2));

    std::print("02-bst-map: all assertions passed\n");
    return 0;
}
