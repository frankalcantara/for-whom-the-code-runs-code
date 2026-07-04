// cap02/09-histogram-exercise.cpp
// Solved exercise: Frequency Analysis
//
// Problem: given an array of n integers in [1, 10^6], compute:
//   (a) the element with highest frequency, the mode;
//   (b) the number of values that appear exactly once;
//   (c) all distinct elements in increasing order with their frequencies.
//
// Input:  line 1 = n; line 2 = n integers
// Output: mode and frequency, singleton count, list of (value, frequency)
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o histogram 09-histogram-exercise.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fehistogram.exe 09-histogram-exercise.cpp

#include <algorithm>
#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <print>
#include <ranges>
#include <unordered_map>
#include <vector>

// ---- Type aliases ----
using Vi  = std::vector<int>;
using Vl  = std::vector<long long>;
using Vp  = std::vector<std::pair<int,int>>;  // (value, frequency)

// ---- Benchmark utility (from Chapter 1) ----
template <std::size_t Runs = 7, typename Fn>
long long benchmark(Fn&& fn) {
    static_assert(Runs >= 2);
    namespace chr = std::chrono;
    std::array<long long, Runs> times{};
    for (std::size_t i = 0; i < Runs; ++i) {
        auto t0 = chr::high_resolution_clock::now();
        fn();
        auto t1 = chr::high_resolution_clock::now();
        times[i] = chr::duration_cast<chr::microseconds>(t1 - t0).count();
    }
    return *std::ranges::min_element(times.begin() + 1, times.end());
}

// ============================================================
// Layer 1 — Naive: O(n^2) time, O(1) extra space
// For each distinct element, scan the entire array to count it.
// Correct for n <= 5000; too slow for n = 10^5.
// ============================================================
struct FreqResult {
    int mode, mode_freq, singletons;
    Vp freq_table;  // sorted by value
};

FreqResult solve_naive(const Vi& a) {
    int n = static_cast<int>(a.size());
    // Find distinct values by checking if each element is "first occurrence"
    Vi distinct;
    for (int i = 0; i < n; ++i) {                  // O(n)
        bool first = true;
        for (int j = 0; j < i; ++j) {              // O(n) per element
            if (a[j] == a[i]) { first = false; break; }
        }
        if (first) distinct.push_back(a[i]);
    }
    std::ranges::sort(distinct);                    // O(d log d), d = distinct count

    FreqResult res{};
    res.mode_freq = 0;
    res.singletons = 0;

    for (int val : distinct) {                      // O(d)
        int cnt = 0;
        for (int x : a) if (x == val) ++cnt;       // O(n) per distinct value → O(n*d)
        res.freq_table.push_back({val, cnt});
        if (cnt > res.mode_freq) {
            res.mode_freq = cnt;
            res.mode      = val;
        }
        if (cnt == 1) ++res.singletons;
    }
    return res;                                     // Total: O(n^2) worst case
}

// ============================================================
// Layer 2 — Algorithmic: O(n log n) time, O(n) space
// Sort the array, then count runs of equal elements.
// ============================================================
FreqResult solve_sort(Vi a) {                       // copy: O(n) space
    std::ranges::sort(a);                           // O(n log n)

    FreqResult res{};
    res.mode_freq = 0;
    res.singletons = 0;

    // Count runs using adjacent-equal traversal: O(n)
    int i = 0, n = static_cast<int>(a.size());
    while (i < n) {
        int val = a[i], cnt = 0;
        while (i < n && a[i] == val) { ++i; ++cnt; }
        res.freq_table.push_back({val, cnt});
        if (cnt > res.mode_freq) {
            res.mode_freq = cnt;
            res.mode      = val;
        }
        if (cnt == 1) ++res.singletons;
    }
    return res;                                     // Total: O(n log n)
}

// ============================================================
// Layer 3 — High Performance: O(n) expected time, O(n) space
// Hash map with reserve to avoid rehashing.
// ============================================================
FreqResult solve_hash(const Vi& a) {
    std::unordered_map<int, int> freq;
    freq.reserve(a.size());                         // avoids O(n) rehashing
    freq.max_load_factor(0.25f);                    // fewer collision chains

    for (int x : a) ++freq[x];                     // O(n) expected

    FreqResult res{};
    res.mode_freq = 0;
    res.singletons = 0;

    // Collect into sorted freq_table: O(d log d) where d = distinct count
    res.freq_table.reserve(freq.size());
    for (auto [val, cnt] : freq) {                  // structured binding
        res.freq_table.push_back({val, cnt});
        if (cnt > res.mode_freq) {
            res.mode_freq = cnt;
            res.mode      = val;
        }
        if (cnt == 1) ++res.singletons;
    }
    std::ranges::sort(res.freq_table);              // sort by value: O(d log d)
    return res;                                     // Total: O(n) expected
}

// ---- Helpers ----
void print_result(const FreqResult& r, bool show_table = true) {
    std::print("mode={} (freq={})  singletons={}\n",
               r.mode, r.mode_freq, r.singletons);
    if (show_table) {
        for (auto [val, cnt] : r.freq_table) {
            std::print("  {} -> {}\n", val, cnt);
        }
    }
}

Vi generate_test(int n, int max_val = 1000, unsigned seed = 42) {
    // Simple LCG for reproducibility without <random>
    Vi v(n);
    unsigned s = seed;
    for (auto& x : v) {
        s = s * 1664525u + 1013904223u;
        x = static_cast<int>(s % static_cast<unsigned>(max_val)) + 1;
    }
    return v;
}

int main() {
    // ---- Correctness verification on small input ----
    Vi sample = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::print("=== Small sample correctness check ===\n");
    auto r1 = solve_naive(sample);
    auto r2 = solve_sort(sample);
    auto r3 = solve_hash(sample);

    // All three must agree
    if (r1.mode_freq != r2.mode_freq || r2.mode_freq != r3.mode_freq ||
        r1.singletons != r2.singletons || r2.singletons != r3.singletons ||
        r1.freq_table != r2.freq_table || r2.freq_table != r3.freq_table) {
        std::print("CORRECTNESS ERROR: algorithms disagree\n");
        return 1;
    }
    std::print("All three agree:\n");
    print_result(r3);

    // ---- Performance benchmark ----
    std::print("\n=== Benchmark ===\n");
    std::print("{:>8}  {:>12}  {:>12}  {:>12}\n",
               "n", "naive [µs]", "sort [µs]", "hash [µs]");
    std::print("{}\n", std::string(54, '-'));

    constexpr long long LIMIT_US = 1'000'000;

    for (int n : {1'000, 10'000, 100'000, 1'000'000}) {
        auto data = generate_test(n);

        long long t_naive = -1;
        if (n <= 10'000)
            t_naive = benchmark([&] { solve_naive(data); });

        long long t_sort = benchmark([&] { solve_sort(data); });
        long long t_hash = benchmark([&] { solve_hash(data); });

        auto naive_str = (t_naive < 0) ? std::string("skip")
                                       : std::format("{}", t_naive);
        std::print("{:>8}  {:>12}  {:>12}  {:>12}\n",
                   n, naive_str, t_sort, t_hash);

        // Verify sort and hash agree on large inputs
        if (n <= 100'000) {
            auto rs = solve_sort(data);
            auto rh = solve_hash(data);
            if (rs.mode_freq != rh.mode_freq || rs.singletons != rh.singletons) {
                std::print("CORRECTNESS ERROR at n={}\n", n);
                return 1;
            }
        }
    }

    // ---- stdin mode: read actual input ----
    // Uncomment to run as a contest submission:
    //
    // int n; std::cin >> n;
    // Vi a(n);
    // for (auto& x : a) std::cin >> x;
    // auto r = solve_hash(a);
    // std::print("{} {}\n{}\n", r.mode, r.mode_freq, r.singletons);
    // for (auto [val, cnt] : r.freq_table)
    //     std::print("{} {}\n", val, cnt);
}
