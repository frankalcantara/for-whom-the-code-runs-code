// Exercise T10.2 - The Numerologist's Street
// Session: T10.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 10 (sieve of Eratosthenes), with chapter 01's
//   cache-locality lesson making a guest appearance
// Tags: chapter-10, chapter-01, number-theory, sieve
//
// MODEL: primality for ALL numbers up to n at once. One number's
//   primality costs O(sqrt n) by trial division; n numbers cost
//   O(n sqrt n) = 3e10 at n = 1e7. The sieve inverts the loop: instead
//   of asking each number for its divisors, each prime crosses out its
//   multiples - the composite numbers pay, once per prime factor below
//   sqrt(n).
// MATH: mark composite[j] for j = p^2, p^2 + p, ... for each p found
//   prime in increasing order. Starting at p^2 is sound because any
//   smaller composite multiple qp (q < p) has a prime factor <= q < p
//   and was crossed out earlier. The total work is
//   sum over primes p <= n of n/p = n * sum 1/p = O(n log log n) -
//   Mertens' theorem, the most useful "almost linear" in the book.
// PROOF: invariant - when the outer loop reaches i, the flag of every
//   number < i^2... more precisely, every composite with a prime
//   factor < i is already marked. So an unmarked i has no prime factor
//   below it: i is prime. Conversely every composite c = pq with
//   p <= sqrt(c) gets marked when the loop processes p. Both
//   directions by strong induction on i.
// COMPLEXITY: O(n log log n) time, O(n) bits of memory -
//   vector<bool> packs 1e7 flags into 1.25 MB, which fits in L2/L3
//   and is the difference between this sieve and a byte-array one at
//   the cache line level (Chapter 1's lesson: the same algorithm, 8x
//   the traffic).
// TYPES: indices fit int (1e7 < 2^31); the count fits int (664,579
//   primes below 1e7; the n / ln n estimate predicts ~620k and
//   undershoots, as it always does at humble n). The marking loop's induction
//   variable j = i * i must be long long-safe... at n = 1e7, i <=
//   3163 so i*i <= 1e7 fits int, but the habit of writing
//   1LL * i * i costs nothing and survives n = 1e9 variants.
// ALTERNATIVES: the linear sieve (each composite crossed exactly once
//   via smallest prime factor) is O(n) and stores SPF for
//   factorization queries - T10.5's tool; segmented sieves handle
//   n = 1e12 ranges in O(sqrt n) memory; Miller-Rabin tests ONE
//   number fast and is the wrong shape for "all of them".
// EDGE CASES: n = 1 (no primes - print 0); n = 2 (one prime, itself);
//   n prime (it is the maximum); n = 1e7 (time and memory budget).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o street t10-s2-1-numerologists-street.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Festreet.exe t10-s2-1-numerologists-street.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<bool> composite(static_cast<std::size_t>(n) + 1, false);
    int count = 0;
    int largest = 0;
    for (int i = 2; i <= n; ++i) {
        if (composite[static_cast<std::size_t>(i)]) continue;
        ++count;
        largest = i;
        for (long long j = 1LL * i * i; j <= n; j += i) {
            composite[static_cast<std::size_t>(j)] = true;
        }
    }

    if (count == 0) {
        std::print("0\n");
    } else {
        std::print("{} {}\n", count, largest);
    }
    return 0;
}
