// Exercise A8.1 - The Council Rotation
// Session: A8, exercise 1 of 10
// Theme: chapter 15 (Josephus recurrence)
// Tags: chapter-15, dynamic-programming, modular-arithmetic
//
// MODEL: survivor of the Josephus elimination process.
// MATH: J(1)=0 and J(i)=(J(i-1)+k) mod i for zero-based indexing.
// PROOF: induction. Removing one member rotates the remaining circle
//   by k positions; undoing that rotation gives the recurrence.
// COMPLEXITY: O(n) time and O(1) memory.
// TYPES: J+k needs int64_t before the modulo; the final index fits
//   within n.
// ALTERNATIVES: explicit simulation is slower and more complex.
// EDGE CASES: n=1; k=1; k>n.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o rotation a8-01-council-rotation.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Ferotation.exe a8-01-council-rotation.cpp

#include <cstdint>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::int64_t k = 0;
    std::cin >> n >> k;
    std::int64_t survivor = 0;
    for (int size = 2; size <= n; ++size) {
        survivor = (survivor + k) % size;
    }
    std::print("{}\n", survivor + 1);
    return 0;
}
