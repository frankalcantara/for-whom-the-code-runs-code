// Exercise 2.7 - Every Third Footstep
// Print indices 0, 3, 6, ... with their values.
// Time: O(ceil(n / 3)). Extra space: O(1).

#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<long long> inscriptions(n);
    for (auto& value : inscriptions) std::cin >> value;

    for (int index = 0; index < n; index += 3)
        std::print("{} {}\n", index, inscriptions[index]);
}
