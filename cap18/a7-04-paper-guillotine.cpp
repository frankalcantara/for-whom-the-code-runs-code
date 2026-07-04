// Exercise A7.4 - The Paper Guillotine
// Session: A7, exercise 4 of 9
// Theme: chapter 04 (binary search on the answer)
// Tags: chapter-04, binary-search, greedy
//
// MODEL: split a fixed sequence into at most k consecutive groups
//   while minimizing the maximum group sum.
// MATH: for a candidate limit X, greedily form the longest current
//   batch whose sum stays <= X, then start a new batch. This uses
//   the fewest batches possible under X. Feasibility is monotone:
//   if X works, any larger limit also works.
// PROOF: exchange argument. Whenever the greedy keeps a job in the
//   current batch, any valid solution with the same previous cuts
//   can also keep it without exceeding X. Therefore cutting earlier
//   cannot reduce the number of batches needed. The greedy batch
//   count is minimal for X, and binary search finds the first
//   feasible X.
// COMPLEXITY: O(n log sum) time and O(1) extra memory.
// TYPES: total pages can reach 2e14, so long long is required for
//   sums and the answer.
// ALTERNATIVES: DP over cuts is too slow at n = 2e5; sorting jobs
//   would break the fixed-order contract.
// EDGE CASES: k = 1 (sum of all pages); k = n (maximum single job);
//   one enormous job; all jobs equal.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o guillotine a7-04-paper-guillotine.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feguillotine.exe a7-04-paper-guillotine.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

namespace {

bool feasible(const std::vector<long long>& pages, int k, long long limit) {
    int batches = 1;
    long long current = 0;
    for (const long long pages_in_job : pages) {
        if (pages_in_job > limit) return false;
        if (current + pages_in_job <= limit) {
            current += pages_in_job;
        } else {
            ++batches;
            current = pages_in_job;
        }
    }
    return batches <= k;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::vector<long long> pages(n);
    long long low = 0;
    long long high = 0;
    for (long long& value : pages) {
        std::cin >> value;
        low = std::max(low, value);
        high += value;
    }

    while (low < high) {
        const long long mid = low + (high - low) / 2;
        if (feasible(pages, k, mid)) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    std::print("{}\n", low);
    return 0;
}
