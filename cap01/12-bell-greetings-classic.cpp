// Exercise 1.3 - The Festival of Paired Bells
// Classic spelling: count unordered pairs among n bell keepers.
// Time: O(1). Space: O(1).

#include <iostream>
#include <print>

int main() {
    long long n = 0;
    std::cin >> n;

    // The expression is evaluated in long long because n is long long; otherwise
    // the multiplication could overflow before the division by two.
    long long greetings = n * (n - 1) / 2;

    std::print("{}\n", greetings);
}
