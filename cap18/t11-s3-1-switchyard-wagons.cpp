// Exercise T11.3 - The Switchyard Wagons
// Session: T11.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 11 (Catalan numbers)
// Tags: chapter-11, number-theory, modular-arithmetic
//
// MODEL: stack-sortable output sequences. The siding is a stack:
//   wagons are pushed in order 1..n, pops may interleave. The
//   number of distinct pop sequences of a stack fed 1..n is the
//   n-th Catalan number C_n.
// MATH: encode each schedule as a string of n pushes and n pops
//   where every prefix has at least as many pushes as pops - a
//   balanced sequence. Distinct schedules give distinct outputs and
//   vice versa, so the count is C_n = C(2n, n) / (n + 1), computed
//   modulo p with factorials and a Fermat inverse (division becomes
//   multiplication by the inverse of (n+1), p is prime and
//   n + 1 < p).
// PROOF: balanced push/pop strings biject with outputs - given the
//   string, the output is determined; given an output, the schedule
//   is forced greedily (pop whenever the needed wagon is on top -
//   any delay only reorders later wagons identically or invalidly).
//   C(2n, n)/(n+1) counts balanced strings by the reflection
//   argument: paths from 0 of 2n steps +-1 that dip below zero
//   biject with paths to -2, of which there are C(2n, n-1);
//   C(2n, n) - C(2n, n-1) simplifies to the closed form.
// COMPLEXITY: O(n) table build, O(1) query, O(n) memory.
// TYPES: factorial table up to 2n = 2e6 of residues; products of
//   two residues ~1e18 in long long. Table index 2e6: size_t.
// ALTERNATIVES: the recurrence C_{t+1} = sum C_i C_{t-i} is O(n^2);
//   C_{t+1} = C_t * 2(2t+1)/(t+2) is O(n) with one inverse per
//   step or a batch inverse - equal asymptotics, more code. The
//   closed form with factorial tables is the contest default.
// EDGE CASES: n = 1 (one schedule: push, pop -> 1); n = 2 (both
//   orders reachable -> 2); n = 3 (5, the example, with 312 the
//   canonical forbidden pattern); n = 1e6 (table of 2e6+1 entries).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o wagons t11-s3-1-switchyard-wagons.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fewagons.exe t11-s3-1-switchyard-wagons.cpp

#include <iostream>
#include <print>
#include <vector>

namespace {

constexpr long long kMod = 1'000'000'007;

long long power_mod(long long base, long long exponent) {
    long long result = 1;
    base %= kMod;
    while (exponent > 0) {
        if ((exponent & 1) == 1) result = result * base % kMod;
        base = base * base % kMod;
        exponent >>= 1;
    }
    return result;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n = 0;
    std::cin >> n;

    const std::size_t size = 2 * static_cast<std::size_t>(n) + 1;
    std::vector<long long> fact(size, 1);
    for (std::size_t i = 1; i < size; ++i) {
        fact[i] = fact[i - 1] * static_cast<long long>(i) % kMod;
    }
    std::vector<long long> inv_fact(size, 1);
    inv_fact[size - 1] = power_mod(fact[size - 1], kMod - 2);
    for (std::size_t i = size - 1; i > 0; --i) {
        inv_fact[i - 1] = inv_fact[i] * static_cast<long long>(i) % kMod;
    }

    const auto un = static_cast<std::size_t>(n);
    const long long central =
        fact[2 * un] * inv_fact[un] % kMod * inv_fact[un] % kMod;
    const long long catalan = central * power_mod(n + 1, kMod - 2) % kMod;
    std::print("{}\n", catalan);
    return 0;
}
