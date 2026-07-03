// Exercise A2.3 - The Cooper's Stock
// Session: A2, exercise 3 of 4 (5 hours for the evaluation)
// Theme: chapter 01 revision (bottleneck arithmetic at the edge of the type)
// Tags: chapter-01
//
// MODEL: barrels = min(S / s, H / h); leftovers are the stocks minus the
//   consumption of that many barrels.
// MATH: barrel number b is buildable iff b*s <= S and b*h <= H, i.e.
//   b <= floor(S/s) and b <= floor(H/h). The largest such b is the minimum
//   of the two floors: the scarcer resource, measured in barrels, decides.
//   This is the bottleneck principle in its smallest clothes.
// PROOF: both constraints are monotone in b, so the feasible set is an
//   initial segment {0, 1, ..., min of the floors}; its maximum is that
//   minimum. Leftovers follow by substitution: S - b*s and H - b*h, both
//   non-negative by construction of b.
// COMPLEXITY: O(1) time and memory.
// TYPES: everything reaches 1e18, so all variables are long long, and the
//   ORDER of operations is the exercise: computing b first keeps every
//   product b*s and b*h bounded by the original stock (b <= S/s implies
//   b*s <= S), so the multiplications cannot overflow. Testing candidate
//   b values by multiplying first would overflow before comparing.
// ALTERNATIVES: simulating barrel by barrel is O(b) with b up to 1e18;
//   floating-point division introduces rounding exactly where the floor
//   must be exact. Integer division already is the floor for non-negative
//   operands.
// EDGE CASES: a stock of zero (zero barrels, full leftovers); stock
//   smaller than one barrel's need; exact consumption (leftover zero);
//   s or h equal to 1 with maximal stocks (the overflow bait).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o cooper a2-03-cooper-stock.cpp

#include <algorithm>
#include <iostream>
#include <print>

int main() {
    long long staves = 0;
    long long hoops = 0;
    long long s = 0;
    long long h = 0;
    std::cin >> staves >> hoops >> s >> h;

    long long barrels = std::min(staves / s, hoops / h);
    std::print("{} {} {}\n", barrels,
               staves - barrels * s, hoops - barrels * h);
    return 0;
}
