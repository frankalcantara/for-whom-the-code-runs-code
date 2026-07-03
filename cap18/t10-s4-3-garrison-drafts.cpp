// Exercise T10.7 - The Garrison Drafts
// Session: T10.S4, exercise 3 of 3 (2 hours for the three; this is the hard one)
// Theme: chapter 10 (binomials mod p: factorial tables, Fermat
//   inverses, the inverse-factorial cascade), integrating
//   exponentiation (T10.3) and chapter 03's output discipline
// Tags: chapter-10, chapter-03, number-theory, modular-arithmetic, fast-exponentiation
//
// MODEL: C(n, k) = n! / (k! (n-k)!) under a prime modulus p. Division
//   does not exist mod p; multiplication by the modular inverse does,
//   and p = 1e9+7 being prime makes every nonzero residue invertible.
// MATH: precompute fact[i] = i! mod p for i <= N = 1e6. By Fermat's
//   little theorem, a^(p-2) = a^(-1) (mod p) for a not divisible by
//   p, so inv_fact[N] = fact[N]^(p-2) via fast exponentiation - ONE
//   exponentiation - and the cascade inv_fact[i-1] = inv_fact[i] * i
//   fills the rest in O(N): correct because (1/i!) * i = 1/(i-1)!.
//   Each query is then two multiplications.
// PROOF: Fermat: the nonzero residues form a group of order p-1
//   under multiplication mod p, so a^(p-1) = 1 and a^(p-2) = a^(-1).
//   No factorial up to 1e6 is divisible by p ~ 1e9 (all factors are
//   smaller than p), so every fact[i] is invertible and the identity
//   C(n,k) * k! * (n-k)! = n! holds mod p with all terms nonzero -
//   the table evaluation is exact, not approximate-then-reduced.
//   The k > n case is 0 by the combinatorial definition, checked
//   before touching the tables.
// COMPLEXITY: O(N + log p) preprocessing, O(1) per query, O(N)
//   memory (two long long tables, 16 MB). The naive per-query product
//   of k terms is O(k) = 1e6 per query: 2e11 at the limits. The
//   per-query Fermat inverse (no cascade) costs O(log p) ~ 30
//   multiplications per query - acceptable, 30x slower, and the
//   cascade exists precisely to delete it.
// TYPES: residues < p ~ 1e9; products of two residues < 1e18: long
//   long throughout, no __int128 needed. The tables hold long long
//   (residues), indices int. Inputs n, k fit int.
// ALTERNATIVES: Pascal's triangle is O(N^2) memory - 4 TB, no;
//   Lucas' theorem handles n up to 1e18 for SMALL p (not needed: n
//   <= 1e6 < p); precomputing inverses of 1..N by the p % i
//   recurrence builds the same tables without exponentiation at all
//   - equivalent, slightly more mysterious, worth knowing when the
//   exponentiation routine is not already on the page.
// EDGE CASES: k = 0 and k = n (both 1); k > n (0); n = 0 (C(0,0) =
//   1); the maximum query C(1e6, 5e5) (deep middle of the table);
//   repeated identical queries (O(1) each makes caching pointless).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o drafts t10-s4-3-garrison-drafts.cpp

#include <cstdio>
#include <string>
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
    constexpr int kMax = 1'000'000;

    std::vector<long long> fact(static_cast<std::size_t>(kMax) + 1, 1);
    for (int i = 1; i <= kMax; ++i) {
        fact[static_cast<std::size_t>(i)] = fact[static_cast<std::size_t>(i) - 1] * i % kMod;
    }
    std::vector<long long> inv_fact(static_cast<std::size_t>(kMax) + 1, 1);
    inv_fact[static_cast<std::size_t>(kMax)] =
        power_mod(fact[static_cast<std::size_t>(kMax)], kMod - 2);
    for (int i = kMax; i >= 1; --i) {
        inv_fact[static_cast<std::size_t>(i) - 1] = inv_fact[static_cast<std::size_t>(i)] * i % kMod;
    }

    int q = 0;
    if (std::scanf("%d", &q) != 1) return 1;

    std::string out;
    out.reserve(static_cast<std::size_t>(q) * 11);
    while (q-- > 0) {
        int n = 0;
        int k = 0;
        if (std::scanf("%d %d", &n, &k) != 2) return 1;
        long long answer = 0;
        if (k <= n) {
            answer = fact[static_cast<std::size_t>(n)] *
                     inv_fact[static_cast<std::size_t>(k)] % kMod *
                     inv_fact[static_cast<std::size_t>(n - k)] % kMod;
        }
        out += std::to_string(answer);
        out.push_back('\n');
    }
    std::fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
