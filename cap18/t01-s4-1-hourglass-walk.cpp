// Exercise T01.5 - The Hourglass Walk
// Session: T01.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 01 (counting iterations before coding, logarithmic decay)
// Tags: chapter-01, simulation
//
// MODEL: f(g) = number of steps of "if even, halve; if odd, subtract 1"
//   until g reaches 0.
// MATH: the bet is about the step count, so count it before simulating.
//   An odd step is always followed by an even value, so at least every
//   second step halves the value. A value g survives at most
//   floor(log2 g) halvings, hence
//     f(g) <= 2 * floor(log2 g) + 2 <= 2 * 60 + 2 = 122
//   for g <= 1e18. Direct simulation is therefore O(log g) per query and
//   the total work is bounded by about 2.5e7 cheap operations: the naive
//   loop is the *right* algorithm, but only after this estimate says so.
// PROOF: termination and the bound follow from the halving argument above;
//   the step counter equals f(g) by construction (each loop iteration
//   performs exactly one legal step).
// COMPLEXITY: O(log g) per hourglass, O(1) memory.
// TYPES: g reaches 1e18, so unsigned long long (or long long) is required;
//   the loop never produces a value larger than its input.
// ALTERNATIVES: a closed formula exists (number of bits plus number of set
//   bits, minus one), but bit-counting vocabulary only arrives in Chapter 5;
//   the estimated simulation is the honest Chapter 1 solution.
// EDGE CASES: g = 0 (zero turns, the loop must not execute); g = 1 (one
//   turn); powers of two (halvings only); g = 1e18 (worst case ~90 turns).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o hourglass t01-s4-1-hourglass-walk.cpp

#include <cstdint>
#include <iostream>
#include <print>

int main() {
    int q = 0;
    std::cin >> q;

    while (q-- > 0) {
        std::uint64_t grains = 0;
        std::cin >> grains;

        int turns = 0;
        while (grains > 0) {
            if (grains % 2 == 0) grains /= 2;
            else                 grains -= 1;
            ++turns;
        }

        std::print("{}\n", turns);
    }
    return 0;
}
