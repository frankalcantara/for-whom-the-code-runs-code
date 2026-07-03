// Exercise A3.2 - The Fairground Stages
// Session: A3, exercise 2 of 5 (6 hours for the evaluation)
// Theme: chapter 06 (min-heap of end times; chapter 04 supplies the sort)
// Tags: chapter-06, chapter-04, sorting, greedy
//
// MODEL: minimum number of machines to schedule n half-open intervals
//   [s, e) without overlap on any machine.
// MATH: sort acts by start. Keep a min-heap of the end times of stages
//   in use. For each act: if the earliest-ending stage has end <= s, it
//   is reused (pop, push e); otherwise a new stage is built (push e).
//   The answer is the heap's maximum size, equivalently the number of
//   stages built.
// PROOF: lower bound — at the start minute of any act that forces a new
//   stage, all stages in the heap end strictly after s, so that many
//   acts plus this one are pairwise simultaneously on stage at time s:
//   any schedule needs that many stages (the maximum-overlap argument).
//   Upper bound — the greedy never assigns two overlapping acts to one
//   stage, by the reuse test e_prev <= s. The two bounds meet, so the
//   greedy is exact. The half-open convention makes e = s a legal reuse,
//   exactly as the statement promises ("free at minute e exactly").
// COMPLEXITY: O(n log n): sort plus n heap operations. Memory O(n).
// TYPES: minutes fit in int (1e9); the stage count fits in int. No sums
//   exist to overflow, but the comparison discipline (<= versus <) is
//   the actual boundary content of this exercise.
// ALTERNATIVES: the sweep with +1/-1 events sorted by time (departure
//   before arrival at the same minute) computes maximum overlap in
//   O(n log n) with no heap and is arguably simpler; a multiset of end
//   times with lower_bound reuses the latest-ending compatible stage,
//   equally correct. The heap version generalizes to weighted variants,
//   which is why the chapter teaches it.
// EDGE CASES: all acts disjoint (one stage); all acts simultaneous
//   (n stages); back-to-back acts e = s (one stage, the boundary case);
//   n = 1; identical intervals repeated.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o stages a3-02-fairground-stages.cpp

#include <algorithm>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<std::pair<int, int>> act(n);
    for (auto& [s, e] : act) std::cin >> s >> e;

    std::ranges::sort(act);

    std::priority_queue<int, std::vector<int>, std::greater<>> ends;
    for (const auto& [s, e] : act) {
        if (!ends.empty() && ends.top() <= s) ends.pop();
        ends.push(e);
    }

    std::println("{}", ends.size());
    return 0;
}
