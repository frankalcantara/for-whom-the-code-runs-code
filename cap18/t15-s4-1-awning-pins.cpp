// Exercise T15.5 - The Awning Pins
// Session: T15.S4, exercise 1 of 3 (2 hours for all three)
// Theme: chapter 15 (interval greedy with exchange proof)
// Tags: chapter-15, sorting, greedy
//
// MODEL: choose the minimum number of points so every closed
//   interval contains at least one chosen point.
// MATH: sort intervals by right endpoint. Scan in that order. If
//   the current interval is not already covered by the last chosen
//   pin, place a new pin at its right endpoint.
// PROOF: exchange argument. Consider the first uncovered interval
//   in right-endpoint order, ending at r. Any valid solution must
//   place a pin somewhere inside it. Moving that pin to r still
//   covers this interval and cannot uncover any later interval that
//   the old pin covered, because later intervals end no earlier
//   than r and any interval containing the old pin with left <= old
//   pin <= r also contains r when it has not already ended. Thus
//   there is an optimal solution making the greedy choice.
// COMPLEXITY: O(n log n) time for sorting and O(n) memory.
// TYPES: endpoints fit int64_t. The answer is at most n.
// ALTERNATIVES: dynamic programming is unnecessary; choosing left
//   endpoints fails because it can miss many intervals that a right
//   endpoint covers.
// EDGE CASES: one interval; nested intervals; touching endpoints;
//   negative coordinates; already covered intervals.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o pins t15-s4-1-awning-pins.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<std::pair<std::int64_t, std::int64_t>> intervals(n);
    for (auto& [left, right] : intervals) std::cin >> left >> right;

    std::sort(intervals.begin(), intervals.end(),
              [](const auto& a, const auto& b) {
                  if (a.second != b.second) return a.second < b.second;
                  return a.first < b.first;
              });

    int pins = 0;
    std::int64_t last_pin = -4'000'000'000'000'000'000LL;
    for (const auto [left, right] : intervals) {
        if (last_pin < left) {
            last_pin = right;
            ++pins;
        }
    }

    std::print("{}\n", pins);
    return 0;
}
