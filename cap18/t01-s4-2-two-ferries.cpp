// Exercise T01.6 - The Two Ferries
// Session: T01.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 01 (exploiting an existing invariant; linear beats log-linear)
// Tags: chapter-01, sorting
//
// MODEL: merge two strictly increasing sequences into one increasing
//   sequence of n + m distinct values.
// MATH: both inputs are already sorted; sorting again would discard that
//   information and pay O((n+m) log(n+m)). Comparing the two current front
//   elements and consuming the smaller one produces the global order in
//   exactly n + m steps, because the smallest unplaced value is always at
//   one of the two fronts.
// PROOF: loop invariant. The output built so far contains the k smallest
//   values overall, in order, and each log's unconsumed part is still
//   increasing. The smaller of the two fronts is <= every unconsumed value
//   of both logs, so appending it preserves the invariant. Induction on k;
//   the tails are appended when one log is exhausted.
// COMPLEXITY: O(n + m) time, O(n + m) memory for input and output.
// TYPES: times reach 1e18, so long long. Indices fit in int.
// ALTERNATIVES: concatenate and std::ranges::sort is correct and shorter,
//   at O((n+m) log(n+m)); for these limits both pass, but the merge is the
//   answer to the harbourmaster's actual question ("touch each entry as few
//   times as possible") and reads each value exactly once.
// EDGE CASES: one log much shorter than the other; all of one log smaller
//   than all of the other (the merge degenerates into two appends);
//   n = m = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o ferries t01-s4-2-two-ferries.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<long long> first(n);
    for (auto& x : first) std::cin >> x;

    int m = 0;
    std::cin >> m;
    std::vector<long long> second(m);
    for (auto& x : second) std::cin >> x;

    std::vector<long long> merged;
    merged.reserve(static_cast<std::size_t>(n) + m);

    int i = 0;
    int j = 0;
    while (i < n && j < m)
        merged.push_back(first[i] < second[j] ? first[i++] : second[j++]);
    while (i < n) merged.push_back(first[i++]);
    while (j < m) merged.push_back(second[j++]);

    for (int k = 0; k < static_cast<int>(merged.size()); ++k)
        std::print("{}{}", merged[k],
                   k + 1 == static_cast<int>(merged.size()) ? "\n" : " ");
    return 0;
}
