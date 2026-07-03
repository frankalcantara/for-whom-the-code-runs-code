// Exercise A7.5 - The Glasshouse Aisle
// Session: A7, exercise 5 of 9
// Theme: chapter 05 (two pointers over non-negative values)
// Tags: chapter-05, two-pointers, sliding-window
//
// MODEL: longest contiguous subarray with sum at most S, with all
//   values non-negative.
// MATH: maintain a window [left, right] and its sum. After adding
//   a[right], move left forward while the sum exceeds S. Because
//   all values are non-negative, removing elements from the left is
//   the only way to make the current right endpoint feasible again.
// PROOF: loop invariant. After the shrinking loop, [left, right] is
//   feasible and no earlier left endpoint can be feasible for this
//   same right, otherwise the loop would have stopped earlier. Thus
//   the best window ending at right is known, and scanning all right
//   endpoints finds the global optimum.
// COMPLEXITY: O(n) time and O(1) extra memory; each endpoint moves
//   forward at most n times.
// TYPES: sums can reach 2e14, so long long is used for S and the
//   running sum. The answer length fits int.
// ALTERNATIVES: prefix sums plus binary search is O(n log n); it is
//   valid, but the monotone window is simpler and faster under
//   non-negative values.
// EDGE CASES: S = 0; zero-valued benches; one bench; no positive
//   value fits; the whole row fits.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o aisle a7-05-glasshouse-aisle.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long limit = 0;
    std::cin >> n >> limit;
    std::vector<long long> demand(n);
    for (long long& value : demand) std::cin >> value;

    long long sum = 0;
    int answer = 0;
    int left = 0;
    for (int right = 0; right < n; ++right) {
        sum += demand[right];
        while (left <= right && sum > limit) {
            sum -= demand[left];
            ++left;
        }
        answer = std::max(answer, right - left + 1);
    }

    std::print("{}\n", answer);
    return 0;
}
