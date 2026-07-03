// Exercise T05.1 - The Dredging Crews
// Session: T05.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 05 (difference array; range update, single final read)
// Tags: chapter-05, prefix-sum, difference-array
//
// MODEL: m range additions on an array of n zeros, queried only once, at
//   the end. The object is the difference array D of the final array A:
//   D[i] = A[i] - A[i-1], with A[0] = 0 by convention.
// MATH: adding d to A[l..r] changes exactly two entries of D: D[l] += d
//   and D[r+1] -= d, because inside the interval consecutive elements
//   move together and their difference is untouched. After all updates,
//   A[i] = D[1] + D[2] + ... + D[i]: a prefix sum reconstructs the array.
// PROOF: by linearity. Each contract is a step function (d on [l, r],
//   0 elsewhere), and the difference of a step function is two impulses,
//   at l and at r+1. Summing contracts commutes with differencing, so the
//   accumulated D is the difference of the accumulated A, and one prefix
//   sum inverts it. Induction on i gives the reconstruction identity.
// COMPLEXITY: O(1) per contract, O(n) reconstruction: O(n + m) time,
//   O(n) memory. The naive loop is O(nm) = 4e10 worst case, far past any
//   time limit; the difference array does the same arithmetic in 8e5 ops.
// TYPES: each segment can receive all m contracts: up to 2e5 * 1e9 =
//   2e14, which overflows int (max ~2.1e9) but fits comfortably in
//   long long (max ~9.2e18). D and A are long long throughout.
// ALTERNATIVES: a Fenwick tree or a lazy segment tree supports the same
//   updates with interleaved queries in O(log n) each, but this statement
//   has no interleaved queries, so paying logarithms buys nothing.
// EDGE CASES: contract covering the whole canal (r = n, the impulse at
//   n+1 must not overflow the buffer, hence size n+2); single-segment
//   contract (l = r); all contracts on one segment (maximum accumulation,
//   the overflow case); n = 1, m = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o dredge t05-s1-1-dredging-crews.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<long long> diff(n + 2, 0);
    for (int j = 0; j < m; ++j) {
        int l = 0;
        int r = 0;
        long long d = 0;
        std::cin >> l >> r >> d;
        diff[l] += d;
        diff[r + 1] -= d;
    }

    long long depth = 0;
    for (int i = 1; i <= n; ++i) {
        depth += diff[i];
        std::print("{}{}", depth, i < n ? ' ' : '\n');
    }
    return 0;
}
