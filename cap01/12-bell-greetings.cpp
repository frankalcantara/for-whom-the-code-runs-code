// Exercise 1.3 - The Festival of Paired Bells
// Count unordered pairs among n bell keepers.
// Time: O(1). Space: O(1).

#include <iostream>
#include <print>

int main() {
    long long n = 0;
    std::cin >> n;

    // The formula starts from ordered pairs and divides by two, because each
    // unordered greeting appears once in each direction.
    // n is already wide before the multiplication, so the product is not formed
    // in int and then widened after the damage is done.
    const long long greetings = n * (n - 1) / 2;
    std::print("{}\n", greetings);
}
