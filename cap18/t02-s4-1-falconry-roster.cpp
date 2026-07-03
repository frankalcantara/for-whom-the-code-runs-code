// Exercise T02.5 - The Falconry Roster
// Session: T02.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 02 (records, projections, tuple keys, stable contracts)
// Tags: chapter-02, sorting
//
// MODEL: sort records by the composite key
//   (speed descending, year ascending, name ascending).
// MATH: lexicographic comparison of the tuple (-speed, year, name) realizes
//   exactly that priority order: the first differing component decides.
//   Negating speed turns "descending" into the default ascending order of
//   the tuple comparison.
// PROOF: a projection-based sort orders elements by their projected keys
//   under std::ranges::less; lexicographic tuple order is a strict weak
//   ordering whenever each component order is (integers and strings are),
//   so the sorted output is the contract's order. The three-level tiebreak
//   makes the order total: two distinct falcons with identical keys print
//   identically either way.
// COMPLEXITY: O(n log n) comparisons; O(n) memory for the records.
// TYPES: speed <= 1e9 fits in int, but -speed on int is fragile at the
//   domain edge (INT_MIN has no positive twin); projecting to a long long
//   negation removes the trap for the price of a register. Names are owned
//   std::string objects; the projection returns references via std::tie
//   semantics avoided here for clarity, a copy of a 20-char string per
//   comparison is acceptable at n = 1e5, and the simple tuple is readable.
// ALTERNATIVES: a handwritten three-branch comparator is equivalent and
//   slightly faster (no string copy); sorting indices by projected key
//   avoids moving records. Both are the right move when records grow; here
//   the records are small and the projection states the key directly.
// EDGE CASES: full ties (identical falcons); all speeds equal (year
//   decides); all speeds and years equal (alphabetical roster); n = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o falconry t02-s4-1-falconry-roster.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <tuple>
#include <vector>

struct Falcon {
    std::string name;
    long long speed;
    long long year;
};

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<Falcon> roster(n);
    for (auto& f : roster) std::cin >> f.name >> f.speed >> f.year;

    std::ranges::sort(roster, {}, [](const Falcon& f) {
        return std::tuple{-f.speed, f.year, f.name};
    });

    for (const auto& [name, speed, year] : roster)
        std::print("{} {} {}\n", name, speed, year);
    return 0;
}
