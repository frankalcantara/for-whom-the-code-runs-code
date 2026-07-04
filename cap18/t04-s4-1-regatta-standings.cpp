// Exercise T04.5 - The Regatta Standings
// Session: T04.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 04 (stable sorting: equal keys carry invisible information)
// Tags: chapter-04, sorting
//
// MODEL: reorder records by points descending; equal points preserve the
//   input order.
// MATH: the previous board's order is data that lives in the positions,
//   not in any field. A stable sort is precisely the contract "equal keys
//   keep their relative order", so stable_sort by descending points
//   implements the harbour rule without materializing the old rank.
//   An unstable sort may happen to preserve it today, on this input, with
//   this library: that is an accident, not a record of the first regatta.
// PROOF: by the stability guarantee, two boats with equal points appear
//   in output in their input order, which encodes the previous board; two
//   boats with different points are ordered by the comparator. Both
//   clauses of the harbour rule are therefore satisfied, and only those.
// COMPLEXITY: O(n log n) comparisons; stable_sort may use O(n) auxiliary
//   memory, the documented price of the stability contract.
// TYPES: points fit in int (<= 1e9); the projection negates into long long
//   to keep the descending order free of edge-of-int superstition. Names
//   are owned strings moved, not copied, by the sort's element moves.
// ALTERNATIVES: storing the original index and sorting by the pair
//   (-points, index) with regular sort is equivalent and makes stability
//   explicit at the cost of one extra field; it is the right spelling when
//   the judge's stable_sort memory overhead matters. Sorting by name or
//   re-reading the previous board misreads the contract entirely.
// EDGE CASES: all points equal (output equals input); all points distinct
//   (stability irrelevant, order by points); two equal-point boats
//   adjacent in input but far apart in points rank; n = 1.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o regatta t04-s4-1-regatta-standings.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feregatta.exe t04-s4-1-regatta-standings.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

struct Boat {
    std::string name;
    long long points;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<Boat> board(n);
    for (auto& b : board) std::cin >> b.name >> b.points;

    std::ranges::stable_sort(board, {},
                             [](const Boat& b) { return -b.points; });

    for (const auto& b : board) std::print("{}\n", b.name);
    return 0;
}
