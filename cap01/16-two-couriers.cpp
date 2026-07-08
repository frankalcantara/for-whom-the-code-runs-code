// Exercise 1.7 - The Two Couriers
// Compare two operation-count models.
// Time: O(1). Space: O(1).

#include <cmath>
#include <iostream>
#include <print>

int main() {
    long double n = 0;
    std::cin >> n;

    // The common factor n stays in both printed estimates, while the comparison
    // still reflects the exact formulas given by the problem.
    const long double log_n = std::log2(n);
    const long double north = 3.0L * n * log_n * log_n;
    const long double south = 0.5L * n * std::sqrt(n);

    std::print("{:.0f} {:.0f}\n", north, south);

    // Ranking uses the real estimates; rounding is only a presentation step and
    // should not be allowed to create a false tie.
    if (north < south) std::print("NORTH\n");
    else if (south < north) std::print("SOUTH\n");
    else std::print("TIE\n");
}
