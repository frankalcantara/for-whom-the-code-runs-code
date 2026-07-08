// cap01/01-complexity-benchmark.cpp
// Complete benchmark for Chapter 01: empirical complexity verification.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o complexity_benchmark 01-complexity-benchmark.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecomplexity_benchmark.exe 01-complexity-benchmark.cpp

#include <algorithm>
#include <array>
#include <chrono>
#include <format>
#include <print>
#include <random>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

template <std::size_t Runs = 7, typename Fn>
long long benchmark(Fn&& fn) {
    static_assert(Runs >= 2);
    namespace chr = std::chrono;
    std::array<long long, Runs> times{};

    for (std::size_t i = 0; i < Runs; ++i) {
        // The benchmark wrapper times only the supplied strategy; input
        // generation and correctness checks stay outside the measured region.
        auto t0 = chr::high_resolution_clock::now();
        fn();
        auto t1 = chr::high_resolution_clock::now();
        times[i] = chr::duration_cast<chr::microseconds>(t1 - t0).count();
    }

    // The first run often pays cache and branch-predictor warm-up costs, so the
    // best retained run better represents the algorithm's steady-state floor.
    return *std::ranges::min_element(times.begin() + 1, times.end());
}

int count_distinct_naive(const std::vector<int>& a) {
    int count = 0;
    int n = static_cast<int>(a.size());

    for (int i = 0; i < n; ++i) {
        bool seen = false;

        // Only the prefix matters: if a value appears earlier, the current
        // position is not the representative that should be counted.
        for (int j = 0; j < i; ++j) {
            if (a[j] == a[i]) {
                seen = true;
                break;
            }
        }
        if (!seen) ++count;
    }

    return count;
}

int count_distinct_sort(std::vector<int> a) {
    // The vector is passed by value because sorting is destructive and the
    // benchmark must keep the original data available for the other strategies.
    // Sorting pays n log n comparisons to buy locality: each later duplicate check
    // compares with the previous element instead of probing scattered history.
    std::ranges::sort(a);

    // The returned range starts at the first discarded duplicate; measuring the
    // prefix gives the number of representatives kept by unique.
    auto unique_end = std::ranges::unique(a);
    return static_cast<int>(
        std::ranges::distance(a.begin(), unique_end.begin()));
}

int count_distinct_hash(const std::vector<int>& a) {
    std::unordered_set<int> seen;
    // Reserving makes the benchmark about hashing and memory locality, not about
    // repeated table growth during insertion.
    seen.reserve(a.size());

    // A low load factor reduces collisions so the measured cost is dominated by
    // node allocation and pointer chasing, the behavior discussed in the text.
    seen.max_load_factor(0.25f);
    for (int x : a) seen.insert(x);
    return static_cast<int>(seen.size());
}

std::vector<int> generate(int n, int max_value = 1'000'000, unsigned seed = 42) {
    // A fixed seed gives every strategy the same reproducible workload; changing
    // n changes scale without changing the statistical shape of the data.
    std::mt19937 rng{seed};
    std::uniform_int_distribution<int> dist{1, max_value};
    std::vector<int> v(n);
    std::ranges::generate(v, [&] { return dist(rng); });
    return v;
}

int main() {
    std::print("{:>8}  {:>12}  {:>12}  {:>12}  {:>6}  {:>6}  {:>6}\n",
               "n", "naive us", "sort us", "hash us", "naive", "sort", "hash");
    std::print("{}\n", std::string(78, '-'));

    for (int n : {1'000, 10'000, 100'000, 1'000'000}) {
        auto data = generate(n);

        long long naive_us = -1;
        // The quadratic version is intentionally capped; extrapolation is enough
        // once the measured smaller case already predicts a multi-second run.
        if (n <= 10'000)
            naive_us = benchmark([&] { count_distinct_naive(data); });

        long long sort_us = benchmark([&] { count_distinct_sort(data); });
        long long hash_us = benchmark([&] { count_distinct_hash(data); });

        // The fast hash implementation is used as a reference, then the other
        // strategies are compared against it where their cost is still practical.
        int ref = count_distinct_hash(data);
        if (n <= 10'000) {
            int naive_answer = count_distinct_naive(data);
            int sort_answer = count_distinct_sort(data);
            if (naive_answer != ref || sort_answer != ref) {
                std::print("correctness error at n={}: naive={} sort={} hash={}\n",
                           n, naive_answer, sort_answer, ref);
                return 1;
            }
        }

        constexpr long long limit_us = 1'000'000;
        std::string naive_time = naive_us < 0 ? "skip" : std::format("{}", naive_us);
        std::string naive_result = naive_us < 0 ? "skip" : (naive_us < limit_us ? "pass" : "fail");

        std::print("{:>8}  {:>12}  {:>12}  {:>12}  {:>6}  {:>6}  {:>6}\n",
                   n,
                   naive_time,
                   sort_us,
                   hash_us,
                   naive_result,
                   sort_us < limit_us ? "pass" : "fail",
                   hash_us < limit_us ? "pass" : "fail");
    }
}
