// Exercise T12.2 - The Carillon Phrase
// Session: T12.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 12 (prefix function, border-period duality)
// Tags: chapter-12, number-theory, gcd, string-algorithms
//
// MODEL: shortest p with s = p^k for some k >= 1 - the shortest
//   period of s that divides |s|.
// MATH: let n = |s| and b = pi[n-1], the longest proper border.
//   Candidate period d = n - b. If d divides n, the answer is d;
//   otherwise the answer is n (s repeats nothing shorter than
//   itself, given the whole-number-of-repetitions promise).
// PROOF: (duality) s has a border of length b iff it has a period
//   n - b: shifting s over itself by n - b and matching is the same
//   statement read twice. So b maximal makes d = n - b the SHORTEST
//   period of s. If d | n, then s = (first d chars)^(n/d) and no
//   shorter dividing period can exist (it would be a shorter
//   period, full stop). If d does not divide n, suppose some
//   period d'' < n divided n. Then d'' <= n/2, and d <= d'' gives
//   d + d'' <= n + gcd(d, d''): Fine and Wilf's theorem applies and
//   makes gcd(d, d'') a period too. Minimality of d forces
//   gcd(d, d'') = d, hence d | d'' | n - contradicting d not | n.
//   So no dividing period exists below n, and the answer is n.
// COMPLEXITY: O(n) time, O(n) memory for pi.
// TYPES: indices in std::size_t; n <= 1e6, no arithmetic risk.
// ALTERNATIVES: try every divisor d of n and verify by scan -
//   O(n * tau(n)), ~240 divisors at worst near 1e6: actually
//   viable, and a fine cross-check; Z-function: d is the answer
//   iff z[d] = n - d and d | n, same bound. The border argument is
//   the one worth owning - it reappears in every period problem.
// EDGE CASES: |s| = 1 (1); all letters equal (1); no repetition
//   (answer n, border may still be positive, e.g. abcab -> 5);
//   border exactly n/2 (abab -> 2); n = 1e6 single letter.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o carillon t12-s2-1-carillon-phrase.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    const std::size_t n = s.size();
    std::vector<std::size_t> pi(n, 0);
    for (std::size_t i = 1; i < n; ++i) {
        std::size_t q = pi[i - 1];
        while (q > 0 && s[i] != s[q]) q = pi[q - 1];
        if (s[i] == s[q]) ++q;
        pi[i] = q;
    }

    const std::size_t period = n - pi[n - 1];
    std::print("{}\n", n % period == 0 ? period : n);
    return 0;
}
