// Exercise T15.4 - The Carousel Accounts
// Session: T15.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 15 (maximum circular subarray)
// Tags: chapter-15
//
// MODEL: maximum non-empty contiguous arc sum in a circular array.
// MATH: a best arc either does not wrap, in which case Kadane's
//   maximum subarray solves it, or it wraps, in which case the
//   excluded middle is a minimum subarray and the value is
//   total_sum - minimum_subarray_sum.
// PROOF: exhaustive case split. A circular arc either is a normal
//   interval in the linear order or crosses the boundary. In the
//   wrapping case, the elements not chosen form one non-empty
//   linear interval; maximizing the chosen sum is therefore
//   equivalent to minimizing the excluded interval. The all-negative
//   case must return the largest element, because the chosen arc is
//   non-empty and excluding the whole array is illegal.
// COMPLEXITY: O(n) time and O(1) memory.
// TYPES: sums can reach 2e14 in magnitude, so int64_t is used for
//   values and accumulators.
// ALTERNATIVES: duplicating the array and using constrained prefix
//   minima works but is heavier; enumerating all starts is O(n^2).
// EDGE CASES: all negative values; n = 1; best arc wraps; best arc
//   does not wrap; zeros mixed with negatives.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o carousel t15-s3-2-carousel-accounts.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::int64_t total = 0;
    std::int64_t best_max = 0;
    std::int64_t best_min = 0;
    std::int64_t cur_max = 0;
    std::int64_t cur_min = 0;
    std::int64_t max_value = -4'000'000'000'000'000'000LL;

    for (int i = 0; i < n; ++i) {
        std::int64_t x = 0;
        std::cin >> x;
        total += x;
        max_value = std::max(max_value, x);

        if (i == 0) {
            cur_max = best_max = x;
            cur_min = best_min = x;
        } else {
            cur_max = std::max(x, cur_max + x);
            best_max = std::max(best_max, cur_max);
            cur_min = std::min(x, cur_min + x);
            best_min = std::min(best_min, cur_min);
        }
    }

    if (max_value < 0) {
        std::print("{}\n", max_value);
    } else {
        std::print("{}\n", std::max(best_max, total - best_min));
    }
    return 0;
}
