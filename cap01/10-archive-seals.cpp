// Exercise 1.1 - The Archive of Repeated Seals
// Count distinct catalog seals.
// Time: O(n log n). Space: O(n) for the input array.

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<long long> seals(n);
    for (auto& seal : seals) std::cin >> seal;

    // Adjacent equal codes turn the set-cardinality question into counting group
    // representatives, which avoids the quadratic "have I seen this before?"
    // scan and keeps memory access contiguous.
    std::ranges::sort(seals);

    // unique compacts one representative of each sorted group; the vector size is
    // unchanged, so the returned boundary is the actual answer.
    auto duplicates = std::ranges::unique(seals);
    const auto distinct = std::ranges::distance(seals.begin(), duplicates.begin());

    std::print("{}\n", distinct);
}
