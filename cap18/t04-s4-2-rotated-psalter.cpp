// Exercise T04.6 - The Rotated Psalter
// Session: T04.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 04 (binary search on a structural invariant, not on a key)
// Tags: chapter-04, binary-search
//
// MODEL: a strictly increasing sequence rotated right by t positions;
//   find t, the 0-based index of the minimum, in O(log n).
// MATH: the bound array splits into two increasing runs (one may be
//   empty): a[0..t-1] are the moved pages, all GREATER than a[n-1]; and
//   a[t..n-1] ends at a[n-1]. So the predicate
//     inFinalRun(i) := a[i] <= a[n-1]
//   is false exactly t times and then true through the end: a monotone
//   boolean landscape with one border. Binary search for the first true
//   needs no key to search for, only this invariant.
// PROOF: every moved page exceeds every kept page: the original order was
//   increasing and the rotation moved its last t pages, the t largest
//   values, to the front. Hence inFinalRun is false on [0, t) and true on
//   [t, n). The minimization-form loop (mid rounds down, true narrows hi)
//   converges to the first true, which is the minimum's position, which
//   is t. Distinctness makes the comparison strict and the runs clean.
// COMPLEXITY: O(log n) comparisons after O(n) input; O(n) memory.
// TYPES: psalm numbers reach 1e18: long long. Indices fit in int; the
//   midpoint uses lo + (hi - lo) / 2 by the standing overflow rule.
// ALTERNATIVES: scanning for the first descent is O(n) and the librarian
//   declined; std::ranges::min_element is also O(n) and does not know the
//   array is rotated-sorted, the precise information that pays for the
//   logarithm. Comparing a[mid] with a[lo] instead of a[n-1] has more
//   special cases when the rotation is zero.
// EDGE CASES: rotation 0 (already sorted: predicate true everywhere,
//   answer 0); rotation n-1 (minimum at the last position); n = 1
//   (answer 0); two pages.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o psalter t04-s4-2-rotated-psalter.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<long long> psalm(n);
    for (auto& p : psalm) std::cin >> p;

    const long long last = psalm[n - 1];
    int lo = 0;
    int hi = n - 1;                 // a[n-1] <= a[n-1]: true exists
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (psalm[mid] <= last) hi = mid;   // mid is in the final run
        else                    lo = mid + 1;
    }

    std::print("{}\n", lo);
    return 0;
}
