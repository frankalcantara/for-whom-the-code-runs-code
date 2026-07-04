// cap02/06-algorithms.cpp
// Standard library algorithms: the 15 you reach for in 90% of problems
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o algorithms 06-algorithms.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fealgorithms.exe 06-algorithms.cpp

#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

using Vi  = std::vector<int>;
using Vl  = std::vector<long long>;

int main() {
    Vi v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // ---- Sorting ----
    std::ranges::sort(v);                             // O(n log n), introsort
    std::print("sorted: ");
    for (auto x : v) std::print("{} ", x);
    std::print("\n");                                  // 1 2 3 4 5 6 7 8 9

    // ---- Binary search (requires sorted input) ----
    bool found  = std::ranges::binary_search(v, 5);  // O(log n)
    auto lb_it  = std::ranges::lower_bound(v, 4);    // O(log n) — first >= 4
    auto ub_it  = std::ranges::upper_bound(v, 6);    // O(log n) — first >  6
    std::print("binary_search(5)={} lower_bound(4)={} upper_bound(6)={}\n",
               found,
               static_cast<int>(std::ranges::distance(v.begin(), lb_it)),   // 3
               static_cast<int>(std::ranges::distance(v.begin(), ub_it)));  // 6

    // Count elements in [4, 6] using lower/upper bound: O(log n)
    int in_range = static_cast<int>(std::distance(lb_it, ub_it));
    std::print("elements in [4,6]: {}\n", in_range);  // 3

    // ---- Counting and finding ----
    Vi w = {1, 2, 2, 3, 3, 3, 4};
    std::print("count(3) = {}\n", std::ranges::count(w, 3));              // 3
    std::print("count_if(even) = {}\n",
               std::ranges::count_if(w, [](int x) { return x % 2 == 0; })); // 2

    auto it = std::ranges::find(w, 3);              // O(n) — first occurrence
    std::print("find(3) index = {}\n",
               static_cast<int>(std::ranges::distance(w.begin(), it)));    // 3

    // ---- Reduction ----
    long long total = std::accumulate(v.begin(), v.end(), 0LL);  // O(n)
    std::print("sum = {}\n", total);                              // 45

    // Prefix sums: output[i] = sum of input[0..i]
    Vl prefix(v.size());
    std::partial_sum(v.begin(), v.end(), prefix.begin());         // O(n)
    std::print("prefix[4] = {}\n", prefix[4]);                    // 1+2+3+4+5 = 15

    // ---- Min / Max ----
    auto [mn, mx] = std::ranges::minmax(v);                       // O(n)
    std::print("min={} max={}\n", mn, mx);                        // 1, 9

    // ---- Permutation ----
    Vi p = {1, 2, 3};
    do {
        for (auto x : p) std::print("{}", x);
        std::print(" ");
    } while (std::ranges::next_permutation(p).found);             // O(n * n!)
    std::print("\n");                                              // 123 132 213 231 312 321

    // ---- Rotate: bring element at position k to front ----
    Vi r = {0, 1, 2, 3, 4};
    std::ranges::rotate(r, r.begin() + 2);                        // O(n)
    std::print("rotate by 2: ");
    for (auto x : r) std::print("{} ", x);                        // 2 3 4 0 1
    std::print("\n");

    // ---- iota: fill with consecutive values ----
    Vi seq(6);
    std::iota(seq.begin(), seq.end(), 1);                          // O(n)
    std::print("iota(1..6): ");
    for (auto x : seq) std::print("{} ", x);                      // 1 2 3 4 5 6
    std::print("\n");

    // ---- copy_if: extract elements matching a predicate ----
    Vi evens;
    std::ranges::copy_if(v, std::back_inserter(evens),            // O(n)
                         [](int x) { return x % 2 == 0; });
    std::print("evens: ");
    for (auto x : evens) std::print("{} ", x);                    // 2 4 6 8
    std::print("\n");

    // ---- fill and generate ----
    Vi f(5);
    std::ranges::fill(f, 7);                                       // O(n)
    std::print("fill(7): ");
    for (auto x : f) std::print("{} ", x);                        // 7 7 7 7 7
    std::print("\n");
}
