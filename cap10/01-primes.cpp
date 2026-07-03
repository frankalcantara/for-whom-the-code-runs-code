// cap10/01-primes.cpp
// Primality, the Sieve of Eratosthenes, and factorization.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o primes 01-primes.cpp

#include <cassert>
#include <algorithm>
#include <print>
#include <utility>
#include <vector>

using i64 = long long;

// ============================================================
// Trial-division primality test: O(sqrt(n)).
// Good enough for a few queries up to about 10^12 in contests.
// ============================================================
bool is_prime(i64 n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (i64 d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

// ============================================================
// Sieve of Eratosthenes: finds all primes up to n.
// Complexity: O(n log log n) time, O(n) memory.
// ============================================================
std::vector<int> sieve(int n) {
    std::vector<bool> is_p(n + 1, true);
    is_p[0] = is_p[1] = false;
    // Mark multiples of p starting at p*p.
    for (int p = 2; (i64)p * p <= n; ++p)
        if (is_p[p])
            for (int j = p * p; j <= n; j += p)
                is_p[j] = false;
    std::vector<int> primes;
    for (int p = 2; p <= n; ++p)
        if (is_p[p]) primes.push_back(p);
    return primes;
}

// ============================================================
// Linear sieve: each composite receives its smallest prime factor once.
// Complexity: O(n) time and O(n) memory.
// It also produces smallest_prime_factor (spf) for fast factorization.
// ============================================================
std::vector<int> linear_sieve(int n, std::vector<int>& spf) {
    spf.assign(n + 1, 0);
    std::vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          // i is prime
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if ((i64)i * p > n) break;
            spf[i * p] = p;
            if (i % p == 0) break;  // each composite is touched once
        }
    }
    return primes;
}

// ============================================================
// Factorization using smallest_prime_factor.
// Requires spf precomputed up to n.
// ============================================================
std::vector<std::pair<int,int>> factorize(int n, const std::vector<int>& spf) {
    std::vector<std::pair<int,int>> factors;
    while (n > 1) {
        int p = spf[n], exp = 0;
        while (n % p == 0) { n /= p; ++exp; }
        factors.push_back({p, exp});
    }
    return factors;
}

int main() {
    // is_prime
    assert(is_prime(2));
    assert(is_prime(997));
    assert(!is_prime(1));
    assert(!is_prime(100));
    assert(is_prime(1000000007));

    // sieve: primes up to 30
    auto p30 = sieve(30);
    assert((p30 == std::vector<int>{2,3,5,7,11,13,17,19,23,29}));

    // There are 78498 primes up to 10^6 (OEIS A006880).
    assert(static_cast<int>(sieve(1000000).size()) == 78498);

    // linear sieve
    std::vector<int> spf;
    auto lp = linear_sieve(1000, spf);
    auto s100 = sieve(100);
    assert(std::includes(s100.begin(), s100.end(), s100.begin(), s100.end()));
    // Check that primes up to 100 are a subset of the linear-sieve output.
    assert(std::includes(lp.begin(), lp.end(), s100.begin(), s100.end()));
    assert(spf[12] == 2);               // 12 = 2^2 * 3; smallest factor is 2
    assert(spf[15] == 3);               // 15 = 3 * 5; smallest factor is 3

    // Factorization of 360 = 2^3 * 3^2 * 5.
    auto f = factorize(360, spf);
    assert(f[0] == std::make_pair(2,3));
    assert(f[1] == std::make_pair(3,2));
    assert(f[2] == std::make_pair(5,1));

    std::print("01-primes: all assertions passed\n");
    return 0;
}
