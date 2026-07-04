// Exercise T05.2 - The Fermentation Log
// Session: T05.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 05 (monotonic deque; sliding-window max and min together)
// Tags: chapter-05, sliding-window, monotonic-queue
//
// MODEL: minimize over all windows of length k the value
//   max(t[i..i+k-1]) - min(t[i..i+k-1]).
// MATH: two monotonic deques, one decreasing (front = window max) and one
//   increasing (front = window min), each storing indices. When index i
//   enters, indices with dominated values are popped from the back; when
//   the window slides past index i - k, stale fronts are popped. Each
//   index enters and leaves each deque at most once.
// PROOF: the deque invariant (values strictly decreasing from front to
//   back, indices increasing) is preserved by both operations, and the
//   front is always the maximum of the current window: any window element
//   not in the deque was popped by a younger element at least as large,
//   so it can never be the maximum again while that younger element is
//   alive. Symmetrically for the minimum. Amortized O(1) per step by the
//   two-pop accounting: each index pays for its own insertion and its own
//   single removal.
// COMPLEXITY: O(n) time, O(k) memory for the deques. The naive rescan of
//   each window is O(nk) = 1e12 worst case; a multiset of the window
//   costs O(n log k), correct but needlessly slower than linear.
// TYPES: temperatures fit in int, but the spread max - min reaches
//   2e9 > INT_MAX, so values are read into long long (or the subtraction
//   is done in long long). This program stores int and widens at the
//   subtraction, which is where the danger actually lives.
// ALTERNATIVES: sparse tables give O(1) max and min per window after
//   O(n log n) preprocessing, a fine answer that spends memory to avoid
//   the deque; two heaps with lazy deletion also work and are worse on
//   every axis here.
// EDGE CASES: k = 1 (every spread is 0); k = n (one window); all equal
//   temperatures; strictly increasing temperatures (answer at the
//   flattest tail); negative temperatures and the 2e9 spread.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o ferment t05-s2-1-fermentation-log.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feferment.exe t05-s2-1-fermentation-log.cpp

#include <deque>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<int> t(n);
    for (auto& x : t) std::cin >> x;

    std::deque<int> maxdq;  // indices, values decreasing
    std::deque<int> mindq;  // indices, values increasing
    long long best = -1;

    for (int i = 0; i < n; ++i) {
        while (!maxdq.empty() && t[maxdq.back()] <= t[i]) maxdq.pop_back();
        maxdq.push_back(i);
        while (!mindq.empty() && t[mindq.back()] >= t[i]) mindq.pop_back();
        mindq.push_back(i);

        if (maxdq.front() <= i - k) maxdq.pop_front();
        if (mindq.front() <= i - k) mindq.pop_front();

        if (i >= k - 1) {
            long long spread =
                static_cast<long long>(t[maxdq.front()]) - t[mindq.front()];
            if (best < 0 || spread < best) best = spread;
        }
    }

    std::println("{}", best);
    return 0;
}
