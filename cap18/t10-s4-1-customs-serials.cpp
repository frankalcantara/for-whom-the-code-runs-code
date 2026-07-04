// Exercise T10.5 - The Customs Serials
// Session: T10.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 10 (linear sieve with smallest prime factor;
//   factorization as table lookup)
// Tags: chapter-10, number-theory, sieve
//
// MODEL: many factorization queries on a bounded universe. The shape
//   "q queries, values <= 1e7" flips the economics: pay O(max value)
//   once to build a smallest-prime-factor (SPF) table, then factor
//   each serial by repeated division by spf, each division removing
//   at least one prime factor.
// MATH: the linear sieve fills spf by iterating i = 2..N and, for
//   each known prime p <= spf[i] with i*p <= N, setting
//   spf[i*p] = p. Every composite m is written EXACTLY once, as
//   m = i * spf[m) with i = m / spf(m): the factorization m = p * i
//   with p = spf(m) <= spf(i) is unique, which is what makes the
//   sieve linear rather than log-log-linear.
// PROOF: (sieve) for composite m, let p = spf(m) and i = m / p; then
//   spf(i) >= p (any smaller prime factor of i would divide m and
//   beat p), so the pair (i, p) satisfies the loop's condition and m
//   is assigned spf = p; the p <= spf[i] guard forbids any other
//   pair from touching m. (query) dividing n by spf(n) repeatedly
//   visits its primes in nondecreasing order; counting transitions
//   where the prime changes counts distinct primes. Each n <= 1e7
//   has at most 8 distinct primes (2*3*5*7*11*13*17*19 = 9,699,690),
//   so a query costs at most ~23 divisions (total multiplicity).
// COMPLEXITY: O(N + q log N) time with N = 1e7; the sieve dominates.
//   Memory: spf as vector<int> is 40 MB - within the usual 256 MB,
//   but int32 matters; int64 would double it for nothing.
// TYPES: serials fit int; spf entries fit int; the per-serial count
//   fits in a digit. The output (2e5 numbers) goes through one
//   buffered string - by now a reflex, noted only because the sieve's
//   40 MB makes this program the unit's memory heavyweight.
// ALTERNATIVES: trial division per query is O(sqrt(1e7)) ~ 3e3
//   divisions, times 2e5 queries = 6e8: borderline where the table is
//   comfortable. The classic Eratosthenes with "smallest marker wins"
//   also yields SPF at O(N log log N) - fine; the linear sieve is
//   taught because its every-composite-once invariant generalizes to
//   computing multiplicative functions on the fly.
// EDGE CASES: serial 1 (zero primes); a prime serial (one); a prime
//   power (one); the primorial 9699690 and friends (eight); repeated
//   serials (no caching needed at 23 divisions each).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o serials t10-s4-1-customs-serials.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feserials.exe t10-s4-1-customs-serials.cpp

#include <cstdio>
#include <string>
#include <vector>

int main() {
    constexpr int kMax = 10'000'000;

    // Linear sieve for smallest prime factors.
    std::vector<int> spf(static_cast<std::size_t>(kMax) + 1, 0);
    std::vector<int> primes;
    primes.reserve(700'000);
    for (int i = 2; i <= kMax; ++i) {
        if (spf[static_cast<std::size_t>(i)] == 0) {
            spf[static_cast<std::size_t>(i)] = i;
            primes.push_back(i);
        }
        for (const int p : primes) {
            if (p > spf[static_cast<std::size_t>(i)] || 1LL * i * p > kMax) break;
            spf[static_cast<std::size_t>(i) * static_cast<std::size_t>(p)] = p;
        }
    }

    int q = 0;
    if (std::scanf("%d", &q) != 1) return 1;

    std::string out;
    out.reserve(static_cast<std::size_t>(q) * 3);
    for (int k = 0; k < q; ++k) {
        int s = 0;
        if (std::scanf("%d", &s) != 1) return 1;
        int distinct = 0;
        while (s > 1) {
            const int p = spf[static_cast<std::size_t>(s)];
            ++distinct;
            while (s % p == 0) s /= p;
        }
        if (k > 0) out.push_back(' ');
        out += std::to_string(distinct);
    }
    out.push_back('\n');
    std::fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
