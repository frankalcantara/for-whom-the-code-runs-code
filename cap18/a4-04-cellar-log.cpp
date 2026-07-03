// Exercise A4.4 - The Cellar Log
// Session: A4, exercise 4 of 6 (6 hours for the evaluation)
// Theme: chapter 05 revision (monotonic deque, sliding window minimum),
//   with chapter 03's buffered output at 1e6 scale
// Tags: chapter-05, chapter-03, sliding-window, monotonic-queue
//
// MODEL: sliding window minimum. The structure that answers it in
//   amortized O(1) per step is the monotonic deque: indices whose
//   values are strictly increasing front to back, front = current
//   window's minimum.
// MATH: invariant - the deque holds exactly the indices i in the
//   current window such that no later index in the window has a value
//   <= a[i] (the "useful" candidates). Each arriving index evicts from
//   the back every index with value >= its own (those can never again
//   be a minimum: the newcomer is at least as small and lives longer);
//   the front is evicted when it slides out of the window.
// PROOF: the front is the minimum of the window because every other
//   window element was either evicted by something <= it still in the
//   deque, or sits behind a smaller-or-equal element; induction over
//   arrivals maintains the invariant. Amortization: each index enters
//   the deque once and leaves once, so n arrivals cost O(n) total
//   despite worst-case O(n) single evictions - the chapter's standard
//   potential argument.
// COMPLEXITY: O(n) time, O(w) deque memory. The naive rescan is
//   O(nw) = 1e12 at the limits: not a candidate. Sparse tables answer
//   the same queries in O(1) after O(n log n) build and O(n log n)
//   memory - fine for ARBITRARY windows, overkill for one sliding one.
// TYPES: readings span [-1e9, 1e9], so int holds values and the
//   comparisons; indices fit int. Output volume (up to 1e6 numbers) is
//   the real performance surface: one '\n'-free buffer, written once
//   (Chapter 3's lesson, revised here on purpose).
// ALTERNATIVES: a multiset window (insert/erase per step) gives
//   O(n log w) - acceptable, lazier, 30x the constant; the two-stack
//   minimum trick achieves the same amortized O(1) with friendlier
//   reasoning and double the memory traffic. The deque is the tool
//   the chapter put in the kit.
// EDGE CASES: w = 1 (output = input); w = n (single minimum);
//   strictly increasing readings (front never evicted by value);
//   strictly decreasing (every arrival evicts everything); negative
//   readings throughout; equal readings (>= eviction keeps the
//   newest, which is correct and keeps the deque small).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o cellar a4-04-cellar-log.cpp

#include <cstdio>
#include <deque>
#include <string>
#include <vector>

int main() {
    int n = 0;
    int w = 0;
    if (std::scanf("%d %d", &n, &w) != 2) return 1;

    std::vector<int> a(static_cast<std::size_t>(n));
    for (auto& x : a) {
        if (std::scanf("%d", &x) != 1) return 1;
    }

    std::string out;
    out.reserve(static_cast<std::size_t>(n - w + 1) * 7);

    std::deque<int> useful;  // indices; values strictly increasing front to back
    for (int i = 0; i < n; ++i) {
        while (!useful.empty() &&
               a[static_cast<std::size_t>(useful.back())] >= a[static_cast<std::size_t>(i)]) {
            useful.pop_back();
        }
        useful.push_back(i);
        if (useful.front() <= i - w) {
            useful.pop_front();
        }
        if (i >= w - 1) {
            if (!out.empty()) out.push_back(' ');
            out += std::to_string(a[static_cast<std::size_t>(useful.front())]);
        }
    }
    out.push_back('\n');
    std::fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
