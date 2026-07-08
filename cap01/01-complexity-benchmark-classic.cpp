// cap01/01-complexity-benchmark-classic.cpp
// Empirical complexity benchmark using classic library calls and explicit types.
// Time: varies by strategy. Space: O(n) for the generated input.

#include <algorithm>
#include <chrono>
#include <iostream>
#include <print>
#include <random>
#include <unordered_set>
#include <vector>

typedef int (*Counter)(const std::vector<int>&);

long long benchmark_count(const std::vector<int>& data, Counter counter) {
    long long times[7] = {};
    int checksum = 0;

    for (int run = 0; run < 7; ++run) {
        std::chrono::high_resolution_clock::time_point begin =
            std::chrono::high_resolution_clock::now();
        checksum ^= counter(data);
        std::chrono::high_resolution_clock::time_point end =
            std::chrono::high_resolution_clock::now();

        times[run] =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                .count();
    }

    // Discarding the first run avoids treating cache and predictor warm-up as
    // part of the steady-state cost of the algorithm.
    long long best = times[1];
    for (int i = 2; i < 7; ++i) {
        if (times[i] < best) {
            best = times[i];
        }
    }

    // The checksum makes every counted result observable to the optimizer.
    if (checksum == -1) {
        std::print("{}\n", checksum);
    }

    return best;
}

int count_distinct_naive(const std::vector<int>& a) {
    int count = 0;
    int n = static_cast<int>(a.size());

    for (int i = 0; i < n; ++i) {
        bool seen = false;

        // Only the prefix can prove that the current value is a duplicate; later
        // equal values must not prevent this first representative from counting.
        for (int j = 0; j < i; ++j) {
            if (a[j] == a[i]) {
                seen = true;
                break;
            }
        }

        if (!seen) {
            ++count;
        }
    }

    return count;
}

int count_distinct_sort(const std::vector<int>& data) {
    std::vector<int> a = data;

    // Sorting turns duplicate detection into a local comparison between
    // neighbors, trading extra comparisons for contiguous memory access.
    std::sort(a.begin(), a.end());

    std::vector<int>::iterator unique_end = std::unique(a.begin(), a.end());
    return static_cast<int>(unique_end - a.begin());
}

int count_distinct_hash(const std::vector<int>& a) {
    std::unordered_set<int> seen;

    // Reserving separates the cost of hashing from the accidental cost of
    // repeatedly rebuilding the table while it grows.
    seen.reserve(a.size());

    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        seen.insert(a[i]);
    }

    return static_cast<int>(seen.size());
}

std::vector<int> generate(int n) {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 1000000);
    std::vector<int> v(n);

    for (int i = 0; i < n; ++i) {
        v[i] = dist(rng);
    }

    return v;
}

const char* verdict(long long elapsed_us) {
    return elapsed_us < 1000000 ? "pass" : "fail";
}

int main() {
    std::print("{:>8}  {:>12}  {:>12}  {:>12}  {:>6}  {:>6}  {:>6}\n",
               "n", "naive us", "sort us", "hash us", "naive", "sort",
               "hash");
    std::print("{}\n",
               "------------------------------------------------------------------------------");

    const int sizes[4] = {1000, 10000, 100000, 1000000};

    for (int i = 0; i < 4; ++i) {
        int n = sizes[i];
        std::vector<int> data = generate(n);

        long long naive_us = -1;
        if (n <= 10000) {
            // Past this point the quadratic curve is already large enough that
            // extrapolation is more useful than waiting for the machine.
            naive_us = benchmark_count(data, count_distinct_naive);
        }

        long long sort_us = benchmark_count(data, count_distinct_sort);
        long long hash_us = benchmark_count(data, count_distinct_hash);

        int reference = count_distinct_hash(data);
        if (n <= 10000 &&
            (count_distinct_naive(data) != reference ||
             count_distinct_sort(data) != reference)) {
            std::print("correctness error at n={}\n", n);
            return 1;
        }

        if (naive_us < 0) {
            std::print("{:>8}  {:>12}  {:>12}  {:>12}  {:>6}  {:>6}  {:>6}\n",
                       n, "skip", sort_us, hash_us, "skip", verdict(sort_us),
                       verdict(hash_us));
        } else {
            std::print("{:>8}  {:>12}  {:>12}  {:>12}  {:>6}  {:>6}  {:>6}\n",
                       n, naive_us, sort_us, hash_us, verdict(naive_us),
                       verdict(sort_us), verdict(hash_us));
        }
    }
}
