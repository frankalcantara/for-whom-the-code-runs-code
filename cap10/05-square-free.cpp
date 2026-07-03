// cap10/05-square-free.cpp
// Square-Free Numbers: determine whether a given positive integer N is
// square-free (not divisible by any perfect square other than 1).
//
// Problem (Wipro contest): given T test cases, for each N determine if N
// is square-free. Print "Yes" or "No".
//
// A number is square-free if and only if its prime factorization contains
// no prime with exponent >= 2.
//
// Algorithm:
//   For each prime p from 2 to sqrt(N):
//     If p | N: divide N by p once.
//     If p | N again: N is not square-free (p^2 divides the original).
//
// This trial division runs in O(sqrt(N)) per query.
// For large batches, precompute a sieve up to sqrt(MAX_N) and test only primes.
//
// Complexity per query: O(sqrt(N)).
// Batch with sieve: O(sqrt(MAX_N) * log(log(MAX_N))) precompute + O(pi(sqrt(N))) per query.
//
// Source: Wipro competitive programming contest — "Square Free Numbers" (2024).
//   Original C++ solution rewritten in C++23.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o square_free 05-square-free.cpp

#include <cassert>
#include <cmath>       // std::sqrt
#include <print>
#include <vector>

// O(sqrt(N)) trial division.
// If after dividing by p once, p still divides N, then p^2 | original N.
bool is_square_free(long long n) {
    if (n <= 1) return n == 1;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            n /= p;                   // remove one factor of p
            if (n % p == 0) return false;  // second factor remains: p^2 | original
        }
    }
    return true;
}

// Batch version: use a sieve of primes up to sqrt(MAX_N) to avoid testing composites.
// Faster for many queries with large N.
std::vector<int> sieve_primes(int limit) {
    std::vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = false;
    std::vector<int> primes;
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i]) primes.push_back(i);
    return primes;
}

// Uses precomputed primes; faster when the same primes are reused across queries.
bool is_square_free_sieve(long long n, const std::vector<int>& primes) {
    if (n <= 1) return n == 1;
    for (int p : primes) {
        if ((long long)p * p > n) break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

int main() {
    // Correctness tests
    struct TC { long long n; bool expected; };
    std::vector<TC> tests = {
        {70,    true},   // 2*5*7 — all exponents 1
        {72,    false},  // 2^3 * 3^2: 4|72 and 9|72
        {1,     true},   // 1 is square-free by convention
        {4,     false},  // 2^2
        {6,     true},   // 2*3
        {12,    false},  // 4|12
        {30,    true},   // 2*3*5
        {49,    false},  // 7^2
        {97,    true},   // prime
        {100,   false},  // 4|100
        {210,   true},   // 2*3*5*7
        {1000,  false},  // 8|1000
    };

    auto primes = sieve_primes(100'000);

    for (auto& [n, expected] : tests) {
        bool r1 = is_square_free(n);
        bool r2 = is_square_free_sieve(n, primes);
        assert(r1 == expected);
        assert(r2 == expected);
        std::print("is_square_free({}) = {} OK\n", n, r1 ? "Yes" : "No");
    }

    // Competition-style output: "Yes" / "No"
    // Example:
    //   Input:  70  => Yes
    //   Input:  72  => No
    std::print("05-square-free: all asserts passed.\n");
    return 0;
}
