// Exercise 1.1 - The Archive of Repeated Seals
// Classic spelling: count distinct catalog seals after sorting.
// Time: O(n log n). Space: O(n).

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<long long> seals(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> seals[i];
    }

    // Sorting is the step that makes duplicates local; after it, each distinct
    // code begins exactly one contiguous group, so the problem becomes counting
    // group representatives instead of remembering every value already seen.
    std::sort(seals.begin(), seals.end());

    // unique does not shrink the vector, so the iterator distance to the new
    // logical end is the group count we need to report.
    std::vector<long long>::iterator distinct_end =
        std::unique(seals.begin(), seals.end());

    std::print("{}\n", distinct_end - seals.begin());
}
