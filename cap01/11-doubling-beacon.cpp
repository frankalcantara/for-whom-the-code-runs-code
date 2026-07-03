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

    while (power <= n) {
        ++flashes;
        if (power > n / 2) break;
        power *= 2;
    }

    std::print("{}\n", flashes);
}
