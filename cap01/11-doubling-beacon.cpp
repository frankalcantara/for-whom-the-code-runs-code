// Exercise 1.2 - The Doubling Beacon
// Count powers of two not greater than n.
// Time: O(log n). Space: O(1).

#include <iostream>
#include <print>

int main() {
    unsigned long long n = 0;
    std::cin >> n;

    int flashes = 0;
    unsigned long long power = 1;

    // At the start of each iteration, power is the next flash strength not yet
    // counted; once doubling would exceed n, every later power is invalid too.
    while (power <= n) {
        ++flashes;

        // Test the next doubling with division so the last valid power near the
        // unsigned limit is counted without overflowing first.
        if (power > n / 2) break;
        power *= 2;
    }

    std::print("{}\n", flashes);
}
