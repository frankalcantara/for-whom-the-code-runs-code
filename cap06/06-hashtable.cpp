// cap06/06-hashtable.cpp
// Hash Table -- unordered_map, custom hash, and manual open addressing.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o hashtable 06-hashtable.cpp

#include <cassert>
#include <bit>
#include <climits>
#include <cstdint>
#include <print>
#include <unordered_map>
#include <utility>
#include <vector>

// ============================================================
// Custom hash for pairs.
// The standard hash for std::pair is not defined; without customization,
// unordered_map<pair<int,int>, V> does not compile.
// ============================================================
struct PairHash {
    std::size_t operator()(std::pair<int,int> p) const {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 * 2654435761ULL + 0x9e3779b9ULL + (h1 << 6) + (h1 >> 2));
    }
};

// ============================================================
// Open addressing with linear probing -- manual hash table.
// Often faster than unordered_map for integer keys because it avoids node allocation.
// Capacity must be a power of two and the load factor should stay below about 0.7.
// ============================================================
struct HashMap {
    static constexpr int EMPTY = INT_MIN;
    static constexpr int DEL   = INT_MIN + 1;
    int cap;
    std::vector<int> keys, vals;
    int sz = 0;

    explicit HashMap(int cap = 1 << 16) : cap(cap), keys(cap, EMPTY), vals(cap, 0) {
        assert(cap > 0 && std::has_single_bit(static_cast<unsigned>(cap)));
    }

    int slot(int k) const {
        if (cap == 1) return 0;
        const auto shift = 32 - std::countr_zero(static_cast<unsigned>(cap));
        return static_cast<int>(static_cast<unsigned>(k * 2654435769U) >> shift);
    }

    void insert(int k, int v) {
        int i = slot(k);
        while (keys[i] != EMPTY && keys[i] != DEL && keys[i] != k)
            i = (i + 1) & (cap - 1);
        if (keys[i] != k) ++sz;
        keys[i] = k; vals[i] = v;
    }

    int* find(int k) {
        int i = slot(k);
        while (keys[i] != EMPTY && keys[i] != k)
            i = (i + 1) & (cap - 1);
        return (keys[i] == k) ? &vals[i] : nullptr;
    }
};

// ============================================================
// Two Sum using unordered_map: O(n) vs O(n^2) naive.
// ============================================================
std::pair<int,int> two_sum(const std::vector<int>& a, int target) {
    std::unordered_map<int,int> seen;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        int complement = target - a[i];
        auto it = seen.find(complement);
        if (it != seen.end()) return {it->second, i};
        seen[a[i]] = i;
    }
    return {-1, -1};
}

int main() {
    // PairHash with unordered_map
    using PairMap = std::unordered_map<std::pair<int,int>, int, PairHash>;
    PairMap grid;
    auto k00 = std::make_pair(0, 0);
    auto k13 = std::make_pair(1, 3);
    grid[k00] = 42;
    grid[k13] = 99;
    assert(grid[k00] == 42);
    assert(grid[k13] == 99);
    std::print("PairHash: grid[0,0]={}, grid[1,3]={}\n", grid[k00], grid[k13]);

    // Manual open addressing
    HashMap hm;
    hm.insert(100, 1); hm.insert(200, 2); hm.insert(300, 3);
    assert(*hm.find(100) == 1);
    assert(*hm.find(200) == 2);
    assert(hm.find(999) == nullptr);
    std::print("HashMap: find(200)={}\n", *hm.find(200));

    // Two Sum
    auto ts1 = two_sum({2, 7, 11, 15}, 9);
    assert(ts1.first == 0 && ts1.second == 1);
    std::print("two_sum target=9: indices ({},{})\n", ts1.first, ts1.second);

    auto ts2 = two_sum({3, 2, 4}, 6);
    assert(ts2.first == 1 && ts2.second == 2);
    std::print("two_sum target=6: indices ({},{})\n", ts2.first, ts2.second);

    std::print("06-hashtable: all assertions passed\n");
    return 0;
}
