// Exercise T11.7 - The Scriptorium Inkpots
// Session: T11.S4, exercise 3 of 3 (2 hours for all three; this is
//   the integrator)
// Theme: chapters 11 + 10 (stars and bars with upper bounds via
//   inclusion-exclusion; modular inverses)
// Tags: chapter-11, chapter-10, number-theory, modular-arithmetic
//
// MODEL: solutions of x1 + ... + xk = n with 0 <= xi <= c, counted
//   modulo p.
// MATH: without the cap the count is stars and bars
//   C(n + k - 1, k - 1). Let A_i be the (bad) event x_i >= c + 1.
//   For a fixed set of j bad desks, substitute y_i = x_i - (c+1) on
//   them: solutions of sum y = n - j(c+1), unbounded, i.e.
//   C(n - j(c+1) + k - 1, k - 1). Inclusion-exclusion:
//   answer = sum_{j >= 0, n - j(c+1) >= 0}
//            (-1)^j C(k, j) C(n - j(c+1) + k - 1, k - 1).
//   The loop stops at j <= n / (c+1) (and j <= k): at most
//   n/(c+1) + 1 terms.
// PROOF: inclusion-exclusion over the bad events; the substitution
//   is a bijection between solutions violating a FIXED j-set (at
//   least) and unbounded solutions of the reduced sum, which is
//   exactly the |A_{i1} cap ... cap A_{ij}| the formula needs.
//   Alternating signs then count each solution with e bad
//   coordinates sum_j (-1)^j C(e, j) = [e = 0] times.
// COMPLEXITY: O(n + k) for factorial tables, O(min(k, n/(c+1)))
//   for the sum - worst case c = 0, n <= k: ~5e5 terms. Memory
//   O(n + k).
// TYPES: indices reach n + k - 1 < 1.5e6 (table size); residue
//   products ~1e18 in long long. The subtraction n - j(c+1) is
//   computed in long long BEFORE any cast - j(c+1) reaches ~1e15
//   and must not pass through int. Negative -> loop break, never an
//   array index.
// ALTERNATIVES: DP over desks with prefix-sum window is
//   O(nk) - 5e11 at the limits, dead; generating functions
//   ((1-x^{c+1})^k / (1-x)^k) say the same formula in different
//   clothes; for tiny k, nested loops. IE is the scaling answer.
// EDGE CASES: c = 0 (answer [n == 0]); c >= n (cap inert: plain
//   stars and bars); n = 0 (1 - and the table-size trap: C(k, j)
//   indexes fact[k], which exceeds n + k - 1 when n = 0, so the
//   tables are sized n + k + 1); k = 1 (answer [n <= c]);
//   j(c+1) overflow drill at c = 1e9; alternating sum kept
//   non-negative with a final ((s % p) + p) % p.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o inkpots t11-s4-3-scriptorium-inkpots.cpp

#include <cstddef>
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

std::vector<long long> fact;
std::vector<long long> inv_fact;

void build_tables(std::size_t size) {
    fact.assign(size, 1);
    for (std::size_t i = 1; i < size; ++i) {
        fact[i] = fact[i - 1] * static_cast<long long>(i) % kMod;
    }
    inv_fact.assign(size, 1);
    inv_fact[size - 1] = power_mod(fact[size - 1], kMod - 2);
    for (std::size_t i = size - 1; i > 0; --i) {
        inv_fact[i - 1] = inv_fact[i] * static_cast<long long>(i) % kMod;
    }
}

long long binom(long long top, long long bottom) {
    if (bottom < 0 || bottom > top) return 0;
    const auto t = static_cast<std::size_t>(top);
    const auto b = static_cast<std::size_t>(bottom);
    return fact[t] * inv_fact[b] % kMod * inv_fact[t - b] % kMod;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n = 0;
    long long k = 0;
    long long c = 0;
    std::cin >> n >> k >> c;

    // Largest index used: n + k - 1 in the second binomial, but k in
    // C(k, j) - and k exceeds n + k - 1 when n = 0. Size n + k + 1
    // covers both.
    build_tables(static_cast<std::size_t>(n + k + 1));

    long long sum = 0;
    for (long long j = 0; j <= k; ++j) {
        const long long remaining = n - j * (c + 1);  // j*(c+1) ~ 1e15: ll
        if (remaining < 0) break;
        const long long term =
            binom(k, j) * binom(remaining + k - 1, k - 1) % kMod;
        sum += (j % 2 == 0) ? term : kMod - term;  // keep non-negative
        sum %= kMod;
    }
    std::print("{}\n", sum);
    return 0;
}
