// Exercise 2.1 - The Prime Vault
// Compile-time primality table for codes below 1000.
// Query time: O(1). Table construction: compile time.

#include <array>
#include <iostream>
#include <print>

constexpr bool is_prime(int value) {
    if (value < 2) return false;
    for (int divisor = 2; divisor <= value / divisor; ++divisor)
        if (value % divisor == 0) return false;
    return true;
}

constexpr auto prime_table() {
    std::array<bool, 1000> table{};
    for (int value = 0; value < static_cast<int>(table.size()); ++value)
        table[value] = is_prime(value);
    return table;
}

constexpr auto primes = prime_table();
static_assert(primes[2]);
static_assert(primes[997]);
static_assert(!primes[1]);
static_assert(!primes[999]);

int main() {
    int queries = 0;
    std::cin >> queries;

    while (queries--) {
        int code = 0;
        std::cin >> code;
        std::print("{}\n", primes[code] ? "OPEN" : "CLOSED");
    }
}
