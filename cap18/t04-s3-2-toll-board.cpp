// Exercise T04.4 - The Toll Board
// Session: T04.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 04 (selection without sorting: nth_element)
// Tags: chapter-04, sorting
//
// MODEL: the k-th smallest of n values, nothing else.
// MATH: a full sort answers every order statistic at once for
//   O(n log n); the board displays exactly one. nth_element partially
//   reorders so that position k-1 holds exactly the value it would hold
//   after sorting, in average O(n), and explicitly refuses to sort the
//   two sides. Buying one statistic at the price of all of them is the
//   mistake this exercise exists to unlearn.
// PROOF: nth_element's contract: after the call, the element at the given
//   position equals the sorted sequence's element there, no element
//   before it is greater, none after it is smaller. The k-th cheapest
//   toll is by definition the sorted sequence's element at 0-based
//   position k-1, which is what the board reads.
// COMPLEXITY: average O(n) time; O(n) memory for the tolls (selection
//   needs the values resident: unlike T03's streams, the algorithm
//   rearranges them).
// TYPES: tolls reach 1e18: long long. k fits in int; the 0-based index
//   k-1 is formed in int and used as an iterator offset.
// ALTERNATIVES: full sort, O(n log n), correct and wasteful here;
//   a counting array is hopeless over a 1e18 domain; quickselect by hand
//   reimplements what the library already guards (including the
//   introspective fallback some implementations add).
// EDGE CASES: k = 1 (minimum) and k = n (maximum); duplicated tolls
//   around position k (the order statistic is over the multiset, copies
//   count); n = 1; tolls of zero.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o toll t04-s3-2-toll-board.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetoll.exe t04-s3-2-toll-board.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<long long> toll(n);
    for (auto& t : toll) std::cin >> t;

    std::ranges::nth_element(toll, toll.begin() + (k - 1));
    std::print("{}\n", toll[k - 1]);
    return 0;
}
