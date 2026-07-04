// cap06/03-heap.cpp
// std::priority_queue -- max-heap, min-heap, and custom heap entries.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o heap 03-heap.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feheap.exe 03-heap.cpp

#include <algorithm>
#include <cassert>
#include <functional>
#include <print>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

using Vi = std::vector<int>;
using i64 = long long;

// ============================================================
// std::priority_queue: max-heap by default.
// Operations: push O(log n), top O(1), pop O(log n).
// ============================================================
void demo_maxheap() {
    std::priority_queue<int> pq;
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) pq.push(x);

    // Extracts in decreasing order.
    Vi result;
    while (!pq.empty()) { result.push_back(pq.top()); pq.pop(); }
    assert((result == Vi{9, 6, 5, 4, 3, 2, 1, 1}));
    std::print("max-heap: ");
    for (int x : result) std::print("{} ", x);
    std::print("\n");
}

// ============================================================
// Min-heap: std::priority_queue with std::greater<T>.
// ============================================================
void demo_minheap() {
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) pq.push(x);

    Vi result;
    while (!pq.empty()) { result.push_back(pq.top()); pq.pop(); }
    assert((result == Vi{1, 1, 2, 3, 4, 5, 6, 9}));
    std::print("min-heap: ");
    for (int x : result) std::print("{} ", x);
    std::print("\n");
}

// ============================================================
// Classic problem: k smallest pair sums (a[i], b[j]).
// With n = m = 1000, there are 10^6 pairs -- too many to enumerate eagerly.
// Min-heap with lazy expansion: O(k log k).
// ============================================================
Vi k_smallest_pair_sums(Vi a, Vi b, int k) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    // (sum, i, j): a[i]+b[j]; next candidates are (i, j+1) and (i+1, j).
    using T = std::tuple<int,int,int>;
    std::priority_queue<T, std::vector<T>, std::greater<T>> pq;
    std::set<std::pair<int,int>> visited;

    auto push = [&](int i, int j) {
        if (i < static_cast<int>(a.size()) && j < static_cast<int>(b.size())
            && !visited.count({i, j})) {
            pq.push({a[i]+b[j], i, j});
            visited.insert({i, j});
        }
    };

    push(0, 0);
    Vi result;
    while (k-- && !pq.empty()) {
        auto [s, i, j] = pq.top(); pq.pop();
        result.push_back(s);
        push(i+1, j);
        push(i, j+1);
    }
    return result;
}

// ============================================================
// Custom heap entry with pair (priority, value): Dijkstra-ready.
// ============================================================
void demo_custom_heap() {
    using P = std::pair<int,int>;  // (dist, node)
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    pq.push({10, 1}); pq.push({5, 2}); pq.push({8, 3});
    auto [d, v] = pq.top();
    assert(d == 5 && v == 2);
    std::print("min-heap (dist,node): ({}, {})\n", d, v);
}

int main() {
    demo_maxheap();
    demo_minheap();
    demo_custom_heap();

    auto res = k_smallest_pair_sums({1,3,5}, {2,4,6}, 5);
    assert((res == Vi{3, 5, 5, 7, 7}));
    std::print("k smallest sums (expected 3 5 5 7 7): ");
    for (int x : res) std::print("{} ", x);
    std::print("\n");

    std::print("03-heap: all assertions passed\n");
    return 0;
}
