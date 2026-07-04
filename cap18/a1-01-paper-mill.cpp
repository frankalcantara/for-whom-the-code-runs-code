// Exercise A1.1 - The Paper Mill Quota
// Session: A1, exercise 1 of 3 (4 hours for the evaluation)
// Theme: chapter 01 (integer division semantics, overflow-aware products)
// Tags: chapter-01
//
// MODEL: total = sum over machines of p_i * floor(T / s_i).
// MATH: machine i completes floor(T / s_i) full cycles in the shift; the
//   floor is the contract ("an interrupted cycle produces nothing"). Each
//   complete cycle contributes p_i sheets.
// PROOF: per-machine counting is direct from the definition of integer
//   division: floor(T/s) is the largest q with q*s <= T, exactly the
//   number of completed cycles. Machines are independent, so totals add.
// COMPLEXITY: O(m) time, O(1) memory.
// TYPES: the order of operations is the exercise. T reaches 1e18 and
//   p_i reaches 1e9; the product p_i * T overflows catastrophically, while
//   p_i * (T / s_i) is bounded by the guaranteed final total, because every
//   term is non-negative and no term can exceed a sum of non-negative
//   terms. Divide first, multiply second. The statement's "guaranteed to
//   fit" clause covers the sum and therefore every individual term.
// ALTERNATIVES: simulating cycles is unbounded nonsense at T = 1e18;
//   __int128 would tolerate careless ordering, but the careful ordering
//   costs nothing and the habit transfers to judges without __int128.
// EDGE CASES: s_i == T (exactly one cycle); s_i > T cannot happen by the
//   constraints, but s_i = T = 1 can; a machine with p_i = 1e9 and
//   s_i = 1 contributing the bulk of the total.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o papermill a1-01-paper-mill.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fepapermill.exe a1-01-paper-mill.cpp

#include <iostream>
#include <print>

int main() {
    long long shift = 0;
    int m = 0;
    std::cin >> shift >> m;

    long long total = 0;
    for (int i = 0; i < m; ++i) {
        long long sheets = 0;
        long long cycle = 0;
        std::cin >> sheets >> cycle;
        total += sheets * (shift / cycle);  // divide first: see TYPES
    }

    std::print("{}\n", total);
    return 0;
}
