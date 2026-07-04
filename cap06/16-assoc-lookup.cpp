// cap06/16-assoc-lookup.cpp
// Lookup throughput for static data: std::set (balanced tree) vs
// std::unordered_set (hash table) vs a sorted std::vector searched with
// std::ranges::binary_search. When the keys are known up front and never
// change, the contiguous sorted vector is cache-friendly and competitive with,
// often faster than, the node-based and hashed containers.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o assoc-lookup 16-assoc-lookup.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feassoc-lookup.exe 16-assoc-lookup.cpp

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

using Clock = std::chrono::steady_clock;

template <class F>
static double time_ms(F&& f) {
    auto t0 = Clock::now();
    volatile long long sink = f();
    (void)sink;
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    const int n = 2'000'000;   // distinct keys
    const int q = 2'000'000;   // queries (about half present, half absent)

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0, 100'000'000);

    std::vector<int> keys;
    keys.reserve(n);
    for (int i = 0; i < n; ++i) keys.push_back(dist(rng));
    std::ranges::sort(keys);
    keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

    std::vector<int> queries;
    queries.reserve(q);
    for (int i = 0; i < q; ++i)
        queries.push_back((i & 1) ? keys[dist(rng) % keys.size()] : dist(rng));

    std::set<int> tree(keys.begin(), keys.end());
    std::unordered_set<int> hash(keys.begin(), keys.end());
    hash.reserve(keys.size() * 2);
    // keys is already the sorted vector.

    double t_tree = time_ms([&] {
        long long hits = 0;
        for (int x : queries) hits += tree.contains(x);
        return hits;
    });
    double t_hash = time_ms([&] {
        long long hits = 0;
        for (int x : queries) hits += hash.contains(x);
        return hits;
    });
    double t_vec = time_ms([&] {
        long long hits = 0;
        for (int x : queries) hits += std::ranges::binary_search(keys, x);
        return hits;
    });

    std::println("std::set          : {:.1f} ms", t_tree);
    std::println("std::unordered_set: {:.1f} ms", t_hash);
    std::println("sorted vector     : {:.1f} ms", t_vec);
    return 0;
}
