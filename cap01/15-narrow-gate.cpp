// Exercise 1.6 - The Narrow Gate
// Count values representable by int and sum all submitted values.
// Time: O(n). Space: O(1), excluding input buffering.

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
        sum += value;

        if (value >= std::numeric_limits<int>::min() &&
            value <= std::numeric_limits<int>::max()) {
            ++accepted;
        }
    }

    std::print("{} {}\n", accepted, sum);
}
