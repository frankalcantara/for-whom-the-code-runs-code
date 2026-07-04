// Exercise T07.1 - The Hotel Lift
// Session: T07.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 07 (simulation with state)
// Tags: chapter-07, simulation
//
// MODEL: a one-dimensional walk. The state is a single integer, the
//   current floor; each request applies two deterministic moves to it.
//   The answer is the sum of the lengths of all moves.
// MATH: starting at p_0 = 1, request i = (s_i, t_i) contributes
//   |p_{i-1} - s_i| + |s_i - t_i| and leaves p_i = t_i. The total is
//   a telescoped sum over requests; no reordering is allowed, so there
//   is nothing to optimize, only to follow.
// PROOF: correctness is by induction on requests: if p_{i-1} is the
//   lift's true position before request i, the two absolute differences
//   are exactly the floors traveled serving it, and p_i = t_i is the
//   true position after. The base case is the house rule p_0 = 1.
// COMPLEXITY: O(n) time, O(1) memory beyond input parsing. With
//   n <= 2e5 this is microseconds; the exercise is a warm-up in honest
//   state-keeping, not in speed.
// TYPES: each summand is < 2e9 and there are up to 2e5 of them, so the
//   total can reach ~4e14, far beyond int and unsigned int. The
//   accumulator must be long long (or int64_t); the floors themselves
//   fit in int but are read into long long to keep the arithmetic in
//   one width.
// ALTERNATIVES: none meaningfully different; any attempt to batch or
//   sort requests changes the problem (the order is part of the
//   contract). This is the simulation lesson in its purest form: the
//   program is the rulebook, restated executably.
// EDGE CASES: n = 1; s = t (zero-length ride, contributes only the
//   approach); request starting exactly where the lift is (zero-length
//   approach); maximum floors with maximum n (overflow check on the
//   accumulator).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o lift t07-s1-1-hotel-lift.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Felift.exe t07-s1-1-hotel-lift.cpp

#include <cstdlib>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    long long position = 1;
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long s = 0;
        long long t = 0;
        std::cin >> s >> t;
        total += std::llabs(position - s) + std::llabs(s - t);
        position = t;
    }
    std::print("{}\n", total);
    return 0;
}
