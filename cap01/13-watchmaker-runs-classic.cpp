// Exercise 1.4 - The Watchmaker's Trial Runs
// Classic spelling: discard warm-up and report min, lower median, and max.
// Time: O(r log r). Space: O(r).

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    int runs = 0;
    std::cin >> runs;

    std::vector<long long> measured(runs);
    for (int i = 0; i < runs; ++i) {
        std::cin >> measured[i];
    }

    // The warm-up is removed before sorting so every statistic is computed only
    // from retained measurements.
    measured.erase(measured.begin());

    // Ordering turns min, lower median, and max into positions instead of three
    // separate selection problems.
    std::sort(measured.begin(), measured.end());

    int retained = static_cast<int>(measured.size());
    int middle = (retained - 1) / 2;

    std::print("{} {} {}\n", measured[0], measured[middle],
               measured[retained - 1]);
}
