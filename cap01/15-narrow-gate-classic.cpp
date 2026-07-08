// Exercise 1.6 - The Narrow Gate
// Classic spelling: count values representable by int and sum all inputs.
// Time: O(n). Space: O(1).

#include <iostream>
#include <limits>
#include <print>

int main() {
    int n = 0;
    std::cin >> n;

    int accepted = 0;
    long long sum = 0;

    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;

        // The ledger records every declaration, including values rejected by
        // the old int counter, so accumulation happens before the range test.
        sum += value;

        // Testing the wide value preserves the evidence needed to decide whether
        // the old int counter could have represented the declaration.
        if (value >= std::numeric_limits<int>::min() &&
            value <= std::numeric_limits<int>::max()) {
            ++accepted;
        }
    }

    std::print("{} {}\n", accepted, sum);
}
