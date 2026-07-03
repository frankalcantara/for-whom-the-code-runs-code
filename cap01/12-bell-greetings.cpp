// Exercise 1.3 - The Festival of Paired Bells
// Count unordered pairs among n bell keepers.
// Time: O(1). Space: O(1).

#include <iostream>
#include <print>

int main() {
    long long n = 0;
    std::cin >> n;

    const long long greetings = n * (n - 1) / 2;
    std::print("{}\n", greetings);
}
