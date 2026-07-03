// cap05/06-incremental-update.cpp
// Incremental update pattern: keep an aggregate current in O(1) per query
// instead of recomputing it from scratch in O(n).
//
// Problem: given humidity[], process q operations of the form (val, idx).
// Each operation adds val to humidity[idx] and returns the sum of even values.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o incremental 06-incremental-update.cpp

#include <cassert>
#include <chrono>
#include <numeric>
#include <print>
#include <vector>

using i64 = long long;
using Vi  = std::vector<int>;
using Vll = std::vector<i64>;
using Adj = std::vector<std::pair<int,int>>;  // {value, index}

// ============================================================
// Naive version -- O(q * n).
// For each query, scans the entire array to sum the even values.
// q = 10^5, n = 10^5: 10^10 operations -> infeasible.
// ============================================================
Vll adjust_naive(Vi humidity, const Adj& ops) {
    Vll result;
    result.reserve(ops.size());
    for (auto [val, idx] : ops) {
        humidity[idx] += val;
        // Recomputes everything from scratch: O(n) per query.
        i64 sum = std::accumulate(humidity.begin(), humidity.end(), 0LL,
            [](i64 acc, int v) { return acc + (v % 2 == 0 ? v : 0); });
        result.push_back(sum);
    }
    return result;
}

// ============================================================
// Optimized version -- O(n + q).
// Keeps sum_even updated incrementally.
//
// Observation: when humidity[idx] changes from old to new:
//   - if old was even, subtract old from sum_even
//   - if new is even, add new to sum_even
// Only one subtraction and one addition per query -- O(1).
//
// Initialization: one O(n) pass to compute the initial sum_even.
// Total: O(n + q).
// ============================================================
Vll adjust_incremental(Vi humidity, const Adj& ops) {
    // Initial sum of even values: O(n).
    i64 sum_even = std::accumulate(humidity.begin(), humidity.end(), 0LL,
        [](i64 acc, int v) { return acc + (v % 2 == 0 ? v : 0); });

    Vll result;
    result.reserve(ops.size());
    for (auto [val, idx] : ops) {
        // Remove the old value's contribution.
        if (humidity[idx] % 2 == 0) sum_even -= humidity[idx];

        // Apply the update.
        humidity[idx] += val;

        // Add the new value's contribution.
        if (humidity[idx] % 2 == 0) sum_even += humidity[idx];

        result.push_back(sum_even);
    }
    return result;
}

int main() {
    // Example 1: humidity = {45, 52, 33, 64}, ops = {(5,0),(-20,1),(-14,0),(18,3)}
    // Step by step:
    //   (5,0):   {50,52,33,64} -> even values: 50+52+64 = 166
    //   (-20,1): {50,32,33,64} -> even values: 50+32+64 = 146
    //   (-14,0): {36,32,33,64} -> even values: 36+32+64 = 132
    //   (18,3):  {36,32,33,82} -> even values: 36+32+82 = 150
    Vi h1 = {45, 52, 33, 64};
    Adj ops1 = {{5,0}, {-20,1}, {-14,0}, {18,3}};
    Vll expected1 = {166, 146, 132, 150};

    assert(adjust_naive(h1, ops1)        == expected1);
    assert(adjust_incremental(h1, ops1)  == expected1);

    // Example 2: humidity = {40}, ops = {(12,0)}
    //   (12,0): {52} -> even values: 52
    Vi h2 = {40};
    Adj ops2 = {{12, 0}};
    Vll expected2 = {52};

    assert(adjust_naive(h2, ops2)        == expected2);
    assert(adjust_incremental(h2, ops2)  == expected2);

    // Extended verification: naive == incremental for a mixed operation set.
    Vi h3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Adj ops3 = {{3,0},{-4,5},{7,2},{-1,9},{2,4},{-6,1},{5,7},{-3,3},{1,8},{-2,6}};
    assert(adjust_naive(h3, ops3) == adjust_incremental(h3, ops3));

    // Benchmark: compare running times for q=50000, n=10000.
    constexpr int N = 10000, Q = 50000;
    Vi big(N);
    std::iota(big.begin(), big.end(), 0);  // {0, 1, 2, ..., N-1}
    Adj big_ops;
    big_ops.reserve(Q);
    for (int i = 0; i < Q; ++i)
        big_ops.push_back({i % 7 - 3, i % N});  // alternates parity often

    auto t0 = std::chrono::steady_clock::now();
    auto r_naive = adjust_naive(big, big_ops);
    auto t1 = std::chrono::steady_clock::now();
    auto r_inc = adjust_incremental(big, big_ops);
    auto t2 = std::chrono::steady_clock::now();

    assert(r_naive == r_inc);

    double ms_naive = std::chrono::duration<double,std::milli>(t1 - t0).count();
    double ms_inc   = std::chrono::duration<double,std::milli>(t2 - t1).count();
    std::print("Naive O(q*n):         {:.1f} ms\n", ms_naive);
    std::print("Incremental O(n+q):   {:.1f} ms  ({:.0f}x faster)\n",
               ms_inc, ms_naive / ms_inc);
    std::print("06-incremental-update: all assertions passed.\n");
    return 0;
}
