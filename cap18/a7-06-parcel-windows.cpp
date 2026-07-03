// Exercise A7.6 - The Parcel Windows
// Session: A7, exercise 6 of 9
// Theme: chapter 06 (monotonic deque)
// Tags: chapter-06, sliding-window, monotonic-queue
//
// MODEL: report the minimum value in every fixed-length sliding
//   window.
// MATH: store candidate indices in a deque with increasing weights.
//   Remove indices that leave the window from the front. Before
//   inserting a new index, remove heavier or equal candidates from
//   the back because the new one is at least as good and lives
//   longer.
// PROOF: loop invariant. The deque contains exactly useful
//   candidates for the current suffix, in increasing weight order.
//   The front is inside the current window and no other stored
//   value is smaller, so it is the window minimum. Discarded back
//   elements can never become optimal while the new index remains.
// COMPLEXITY: O(n) time and O(w) memory. Each index enters and
//   leaves the deque once.
// TYPES: weights fit int. Indices fit int. No aggregation is done.
// ALTERNATIVES: a heap needs lazy deletion and O(n log n); a sparse
//   table answers static ranges but is heavier for one left-to-right
//   pass.
// EDGE CASES: w = 1; w = n; equal weights; negative weights;
//   strictly increasing and strictly decreasing sequences.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o windows a7-06-parcel-windows.cpp

#include <deque>
#include <iostream>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int w = 0;
    std::cin >> n >> w;
    std::vector<int> weight(n);
    for (int& value : weight) std::cin >> value;

    std::deque<int> dq;
    for (int i = 0; i < n; ++i) {
        while (!dq.empty() && dq.front() <= i - w) dq.pop_front();
        while (!dq.empty() && weight[dq.back()] >= weight[i]) dq.pop_back();
        dq.push_back(i);
        if (i + 1 >= w) {
            if (i + 1 > w) std::cout << ' ';
            std::cout << weight[dq.front()];
        }
    }
    std::cout << '\n';
    return 0;
}
