// Exercise 1.7 - The Two Couriers
// Classic spelling: compare two operation-count models.
// Time: O(1). Space: O(1).

#include <cmath>
#include <iostream>
#include <print>

int main() {
    long double n = 0;
    std::cin >> n;

    long double log_n = std::log2(n);
    long double north = 3.0L * n * log_n * log_n;
    long double south = 0.5L * n * std::sqrt(n);

    std::print("{:.0f} {:.0f}\n", north, south);

    // Compare the unrounded estimates; output rounding should not manufacture a
    // tie that is not present in the model.
    if (north < south) {
        std::print("NORTH\n");
    } else if (south < north) {
        std::print("SOUTH\n");
    } else {
        std::print("TIE\n");
    }
}
