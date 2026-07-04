// Exercise T11.4 - The Chancery Seals
// Session: T11.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 11 (pigeonhole principle, adversarial worst case)
// Tags: chapter-11
//
// MODEL: worst-case guarantee against an adversary who chooses the
//   draw order. The clerk needs the smallest d such that EVERY
//   d-subset drawn from the sack contains m sticks of one color.
// MATH: the adversary's best stall takes at most min(c_i, m-1)
//   sticks of each color i - more of any color hands over the
//   prize. So the longest prize-free sequence has length
//   S = sum_i min(c_i, m-1), and the answer is S + 1, PROVIDED some
//   color can reach m at all (some c_i >= m); otherwise -1: the
//   sack can be emptied with every color stuck below m.
// PROOF: (upper bound) any S+1 draws exceed the stall budget of
//   some color by pigeonhole - the budgets sum to S - so that
//   color reaches min(c_i, m-1) + 1 = m draws (its budget was the
//   capped value only when c_i >= m... and if c_i < m the budget is
//   c_i, which CANNOT be exceeded; hence the exceeded color has
//   c_i >= m and reaches m). (lower bound) the stall sequence
//   itself - take min(c_i, m-1) of each color - has S draws and no
//   color at m. Both directions tight: answer S + 1.
// COMPLEXITY: O(k) time, O(1) extra memory (stream the counts).
// TYPES: S <= 2e5 * (1e9 - 1) ~ 2e14: overflows int and unsigned,
//   fits long long - the type IS the exercise's trap. m - 1 with
//   m = 1 is 0: min(c_i, 0) = 0, still fine in signed arithmetic.
// ALTERNATIVES: none meaningfully different - this is a closed-form
//   pigeonhole problem; simulation or binary search on d with a
//   feasibility check reach the same number with more code.
// EDGE CASES: m = 1 (any single draw wins: answer 1, since
//   S = 0); no c_i >= m (-1); all c_i < m except one; m > sum c_i
//   handled by the same c_i >= m test per color (the sum does not
//   matter - sticks of OTHER colors never help); k = 1
//   (answer m if c_1 >= m); c_i and m at 1e9.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o seals t11-s3-2-chancery-seals.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feseals.exe t11-s3-2-chancery-seals.cpp

#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long k = 0;
    long long m = 0;
    std::cin >> k >> m;

    long long stall = 0;       // longest prize-free sequence
    bool reachable = false;    // some color can supply m sticks
    for (long long i = 0; i < k; ++i) {
        long long count = 0;
        std::cin >> count;
        stall += std::min(count, m - 1);
        if (count >= m) reachable = true;
    }

    if (!reachable) {
        std::print("-1\n");
    } else {
        std::print("{}\n", stall + 1);
    }
    return 0;
}
