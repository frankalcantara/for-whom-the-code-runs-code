// cap02/02-vectors.cpp
// std::vector: construction, access, common contest operations
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o vectors 02-vectors.cpp

#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

using Vi  = std::vector<int>;
using Vl  = std::vector<long long>;

// ---- Construction patterns ----

Vi make_zero(int n) {                   // n zeros, O(n)
    return Vi(n, 0);
}

Vi make_iota(int n) {                   // 0,1,2,...,n-1, O(n)
    Vi v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

// ---- Aggregate operations ----

long long sum(const Vi& v) {            // O(n)
    return std::accumulate(v.begin(), v.end(), 0LL);
}

int max_element(const Vi& v) {          // O(n) — returns value, not iterator
    return *std::ranges::max_element(v);
}

// ---- Searching ----

// Binary search on sorted vector: O(log n)
// Precondition: v is sorted in non-decreasing order.
bool contains_sorted(const Vi& v, int target) {
    return std::ranges::binary_search(v, target);
}

// First position of target in sorted v (lower_bound): O(log n)
// Returns v.size() if target is not present.
int lower(const Vi& v, int target) {
    auto it = std::ranges::lower_bound(v, target);
    return static_cast<int>(std::ranges::distance(v.begin(), it));
}

// ---- In-place modification ----

void double_all(Vi& v) {                // O(n)
    for (auto& x : v) x *= 2;
}

void reverse_inplace(Vi& v) {           // O(n)
    std::ranges::reverse(v);
}

// Remove duplicates from sorted vector in-place: O(n)
// Returns new logical size; physical vector is not resized.
int deduplicate(Vi& v) {
    auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);
    return static_cast<int>(v.size());
}

// ---- Merging two sorted vectors: O(n + m) ----
Vi merge_sorted(const Vi& a, const Vi& b) {
    Vi result;
    result.reserve(a.size() + b.size());
    std::ranges::merge(a, b, std::back_inserter(result));
    return result;
}

int main() {
    Vi v = {4, 1, 7, 2, 9, 3, 6, 5, 8};

    // Sort and print
    std::ranges::sort(v);                   // O(n log n)
    std::print("sorted: ");
    for (auto x : v) std::print("{} ", x); // 1 2 3 4 5 6 7 8 9
    std::print("\n");

    // Aggregate
    std::print("sum={} max={}\n", sum(v), max_element(v));  // 45, 9

    // Binary search on sorted v
    std::print("contains 5? {}\n", contains_sorted(v, 5)); // true
    std::print("lower_bound(6) = {}\n", lower(v, 6));       // 5 (0-indexed)

    // Deduplicate
    Vi dup = {1, 1, 2, 3, 3, 3, 4};
    int sz = deduplicate(dup);
    std::print("after dedup ({} unique): ", sz);
    for (auto x : dup) std::print("{} ", x);
    std::print("\n");                                        // 1 2 3 4

    // Merge
    Vi a = {1, 3, 5}, b = {2, 4, 6};
    auto merged = merge_sorted(a, b);
    std::print("merged: ");
    for (auto x : merged) std::print("{} ", x);            // 1 2 3 4 5 6
    std::print("\n");

    // iota
    auto iota_v = make_iota(6);
    std::print("iota: ");
    for (auto x : iota_v) std::print("{} ", x);            // 0 1 2 3 4 5
    std::print("\n");
}
