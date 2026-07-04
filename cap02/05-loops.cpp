// cap02/05-loops.cpp
// Loop forms in C++23: index-based, range-based, views, parallel
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feloops.exe 05-loops.cpp

#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

using Vi = std::vector<int>;

int main() {
    Vi v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // ---- 1. Index-based for: when index arithmetic is needed ----
    // Use when: you need i and j simultaneously, or i += step != 1.
    for (int i = 0; i + 1 < static_cast<int>(v.size()); ++i) {
        // access v[i] and v[i+1] — impossible with range-for alone
        (void)(v[i] + v[i+1]);  // suppress unused-value warning
    }

    // ---- 2. Range-based for: default choice for reading ----
    long long total = 0;
    for (int x : v) total += x;                             // O(n)
    std::print("sum = {}\n", total);                         // 55

    // ---- 3. Range-based for with structured bindings (std::views::enumerate) ----
    // C++23: get index and value together without a manual counter.
    std::print("index-value pairs:\n");
    for (auto [i, x] : std::views::enumerate(v)) {          // O(n)
        if (i < 4) std::print("  v[{}] = {}\n", i, x);
    }

    // ---- 4. std::views pipeline — lazy, zero-copy ----
    // Keep only even elements, square them, take the first 3.
    auto pipeline = v
        | std::views::filter([](int x) { return x % 2 == 0; })   // lazy
        | std::views::transform([](int x) { return x * x; })      // lazy
        | std::views::take(3);                                     // lazy

    std::print("even squares (first 3): ");
    for (auto x : pipeline) std::print("{} ", x);  // 4 16 36
    std::print("\n");

    // ---- 5. Reverse view: O(1) setup, O(n) traversal ----
    std::print("reversed: ");
    for (auto x : v | std::views::reverse) std::print("{} ", x);
    std::print("\n");                                        // 10 9 8 ... 1

    // ---- 6. Slide/stride views (C++23) ----
    // Every other element starting from index 0:
    std::print("stride 2: ");
    for (auto x : v | std::views::stride(2)) std::print("{} ", x);
    std::print("\n");                                        // 1 3 5 7 9

    // ---- 7. std::ranges::for_each — algorithm instead of loop ----
    Vi doubled;
    doubled.reserve(v.size());
    std::ranges::transform(v, std::back_inserter(doubled),  // O(n)
                           [](int x) { return x * 2; });
    std::print("doubled[0..4]: ");
    for (auto x : doubled | std::views::take(5)) std::print("{} ", x);
    std::print("\n");                                        // 2 4 6 8 10

    // ---- 8. Parallel for_each: std::execution::par_unseq ----
    // Distributes across CPU cores. Not useful for n < ~10^5 (thread overhead).
    // Safe only when loop body has NO loop-carried dependencies.
    Vi big(1'000'000, 1);
    std::for_each(std::execution::par_unseq,                // parallel + vectorized
                  big.begin(), big.end(),
                  [](int& x) { x *= 3; });                 // O(n) wall time / num_cores
    std::print("big[0] = {} (should be 3)\n", big[0]);

    // ---- 9. do-while: at least one iteration ----
    // Used for: input loops where you read until EOF or sentinel.
    int q = 0;
    std::cin >> q;                                           // read number of queries
    do {
        std::print("query\n");
        --q;
    } while (q > 0);
}
