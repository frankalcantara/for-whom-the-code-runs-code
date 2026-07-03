// Exercise T10.6 - The Junction Timetable
// Session: T10.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 10 (CRT for non-coprime moduli via the extended
//   Euclidean algorithm)
// Tags: chapter-10, number-theory, modular-arithmetic, gcd
//
// MODEL: the system t = r1 (mod a), t = r2 (mod b) - the Chinese
//   Remainder Theorem, in the general (non-coprime) form, because no
//   railway company was consulted about coprimality.
// MATH: let g = gcd(a, b). Solutions exist iff g | (r2 - r1).
//   Extended Euclid gives x, y with a x + b y = g. Then
//   t = r1 + a * x * ((r2 - r1) / g), taken modulo L = lcm(a, b) =
//   a / g * b, is the unique solution in [0, L); all solutions differ
//   by multiples of L.
// PROOF: (necessity) t = r1 + a m = r2 + b n implies r2 - r1 =
//   a m - b n, a multiple of g. (construction) substituting the
//   Bezout identity: a * (x (r2-r1)/g) = (g - b y) (r2-r1)/g =
//   (r2 - r1) - b * (...), so t = r1 (mod a) by construction and
//   t = r2 (mod b) by the line above. (uniqueness mod L) two
//   solutions differ by a common multiple of a and b, and lcm is the
//   least. The code reduces x (r2-r1)/g modulo b/g BEFORE multiplying
//   by a - that step is not pedantry, it is what keeps every product
//   below 1e18 (see TYPES).
// COMPLEXITY: O(log min(a, b)) for the extended Euclid; everything
//   else is arithmetic. There is no loop to optimize - only an
//   identity to not get wrong.
// TYPES: the entire difficulty. a, b <= 1e9 so L <= 1e18: fits long
//   long, barely. The dangerous product is a * k; safe iff k is
//   first reduced mod b/g (<= 1e9), giving a * k < 1e18. The naive
//   x * (r2 - r1) (x up to ~1e9, difference up to ~1e9) also stays
//   under 1e18 only after reduction - hence the explicit mods and
//   the positive-remainder fixups (C++'s % keeps the dividend's
//   sign; CRT wants canonical residues). For moduli near 1e18 the
//   same algorithm needs __int128 multiplication throughout.
// ALTERNATIVES: brute-force stepping the larger period (t = r1, r1+a,
//   ... testing mod b) is O(b/g) - up to 1e9 steps, the desperation
//   move; coprime-only CRT with inverse-by-Fermat fails silently here
//   (b/g is not prime); successive substitution generalizes to k
//   congruences by folding pairs with exactly this routine.
// EDGE CASES: g does not divide r2 - r1 (never); a = b (answer r1 if
//   r1 == r2, else never); r1 = r2 = 0 (answer 0); coprime periods
//   (classic CRT); a = 1 (t = r2); maximum a, b coprime (L ~ 1e18,
//   the overflow drill).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o junction t10-s4-2-junction-timetable.cpp

#include <iostream>
#include <numeric>
#include <print>

namespace {

// Returns gcd(a, b) and fills x, y with a x + b y = gcd.
long long extended_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1 = 0;
    long long y1 = 0;
    const long long g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long a = 0;
    long long r1 = 0;
    long long b = 0;
    long long r2 = 0;
    std::cin >> a >> r1 >> b >> r2;

    long long x = 0;
    long long y = 0;
    const long long g = extended_gcd(a, b, x, y);

    if ((r2 - r1) % g != 0) {
        std::print("never\n");
        return 0;
    }

    const long long step = b / g;          // t is unique mod lcm = a * step
    long long k = (r2 - r1) / g % step;    // multiplier for a, reduced first
    k = k * (x % step) % step;             // both factors < 1e9: product < 1e18
    if (k < 0) k += step;                  // canonical residue
    const long long t = r1 + a * k;        // < a * step + a <= lcm + a: in range
    std::print("{}\n", t);
    return 0;
}
