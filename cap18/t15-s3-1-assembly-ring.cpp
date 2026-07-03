// Exercise T15.3 - The Assembly Ring
// Session: T15.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 15 (Josephus recurrence)
// Tags: chapter-15, dynamic-programming, modular-arithmetic
//
// MODEL: Josephus survivor with n people and step k, reported in
//   one-based indexing.
// MATH: let J(i) be the zero-based survivor among i people. The
//   base case is J(1) = 0. When the circle grows from i - 1 to i,
//   the survivor's index is shifted by k positions, so
//   J(i) = (J(i - 1) + k) mod i.
// PROOF: induction on circle size. After the first removal in a
//   circle of size i, the remaining circle of i - 1 people has the
//   same relative Josephus process, only with the origin rotated by
//   k. The induction hypothesis gives the survivor in that smaller
//   circle; undoing the rotation gives the recurrence.
// COMPLEXITY: O(n) time and O(1) memory.
// TYPES: n <= 1e7 and k <= 1e9. The sum survivor + k fits in
//   int64_t before the modulo operation; the final answer fits int.
// ALTERNATIVES: explicit simulation with a list is O(n log n) or
//   worse depending on structure; the recurrence is the direct
//   state the statement is hiding.
// EDGE CASES: n = 1; k = 1; k > n; the classic n = 7, k = 2.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o ring t15-s3-1-assembly-ring.cpp

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
