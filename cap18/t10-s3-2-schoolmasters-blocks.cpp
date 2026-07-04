// Exercise T10.4 - The Schoolmaster's Blocks
// Session: T10.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 10 (trial-division factorization to sqrt n)
// Tags: chapter-10, number-theory
//
// MODEL: factor a single n <= 1e12. Trial division by candidates up
//   to sqrt(n) <= 1e6 suffices: a million divisions, microseconds.
// MATH: divide out each candidate d = 2, 3, 5, ... while d divides
//   the remainder, recording one factor per division; stop when
//   d * d > remainder; whatever remains above 1 is a single prime.
//   The factors come out nondecreasing because smaller candidates are
//   exhausted before larger ones are tried.
// PROOF: invariant - when candidate d is tried, the remainder has no
//   prime factor < d (all were divided out). Hence (1) any d that
//   divides the remainder is prime: a composite d would have a prime
//   factor < d that also divides the remainder, contradiction; and
//   (2) when d * d exceeds the remainder r > 1, r is prime, because a
//   composite r = xy with x <= y forces x <= sqrt(r) < d, again
//   contradicting the invariant. Multiplying the recorded factors
//   reconstructs n by construction.
// COMPLEXITY: O(sqrt n) divisions worst case (n prime), O(log n)
//   factors at most (each division at least halves... divides by at
//   least 2). The d = 2 case plus odd d cuts the candidate count in
//   half; wheel-30 cuts further; neither changes the exponent, both
//   are free style points.
// TYPES: n reaches 1e12: long long. The loop guard d * d <= r with
//   d, r long long is safe (1e12 fits); writing d <= r / d instead
//   dodges the question entirely and costs one division per
//   candidate. Factor count <= 40 (2^40 > 1e12): a tiny vector.
// ALTERNATIVES: Pollard's rho factors 1e18 in milliseconds and is the
//   tool one chapter of maturity later; an SPF sieve answers MANY
//   queries below 1e7 (T10.5, next door); Fermat's method shines only
//   when n is a product of two close primes. For one n at 1e12, trial
//   division is the correct amount of cleverness: none.
// EDGE CASES: n = 1 (empty product - print 1 per the statement);
//   n prime (single factor, the d*d > r exit does the work); n a
//   prime square (1e12 = (1e6)^2 boundary: d*d == r must enter the
//   loop, hence <=, not <); powers of two; n = 999999999989 (largest
//   prime below 1e12, the slow path).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o blocks t10-s3-2-schoolmasters-blocks.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feblocks.exe t10-s3-2-schoolmasters-blocks.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n = 0;
    std::cin >> n;

    std::vector<long long> factors;
    long long r = n;
    for (long long d = 2; d * d <= r; ++d) {
        while (r % d == 0) {
            factors.push_back(d);
            r /= d;
        }
    }
    if (r > 1) factors.push_back(r);

    if (factors.empty()) {
        std::print("1\n");
        return 0;
    }
    std::string out;
    for (std::size_t i = 0; i < factors.size(); ++i) {
        if (i > 0) out.push_back(' ');
        out += std::to_string(factors[i]);
    }
    std::print("{}\n", out);
    return 0;
}
