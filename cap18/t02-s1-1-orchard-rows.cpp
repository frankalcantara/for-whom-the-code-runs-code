// Exercise T02.1 - The Orchard Rows
// Session: T02.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 02 (1D prefix sums: pay O(n) once, answer each query in O(1))
// Tags: chapter-02, prefix-sum, number-theory
//
// MODEL: static array, q range-sum queries. Build p[0] = 0,
//   p[i+1] = p[i] + a[i]; then sum(l..r) = p[r] - p[l-1] in 1-based terms.
// MATH: p[k] is the sum of the first k values. A range is the difference of
//   two prefixes because addition has an inverse; subtracting the prefix
//   that ends before l removes exactly the unwanted head.
// PROOF: induction on k gives p[k] = a_1 + ... + a_k (build invariant).
//   Then p[r] - p[l-1] = (a_1+...+a_r) - (a_1+...+a_{l-1}) = a_l+...+a_r.
// COMPLEXITY: O(n) build, O(1) per query, O(n + q) total; O(n) memory.
// TYPES: the full sum reaches 2e5 * 1e9 = 2e14, beyond int; prefixes are
//   long long. The accumulation happens in the prefix array's own type, so
//   no intermediate is ever computed in int.
// ALTERNATIVES: scanning each query costs O(nq) = 4e10 visits, an
//   algorithmic confession; a Fenwick tree (Chapter 6) supports updates the
//   problem does not have, and would be machinery without a customer.
// EDGE CASES: l == r (single tree); the full range 1..n; zero-crate trees;
//   q queries repeating the same range.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o orchard t02-s1-1-orchard-rows.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long crates = 0;
        std::cin >> crates;
        prefix[i + 1] = prefix[i] + crates;
    }

    while (q-- > 0) {
        int l = 0;
        int r = 0;
        std::cin >> l >> r;
        std::print("{}\n", prefix[r] - prefix[l - 1]);
    }
    return 0;
}
