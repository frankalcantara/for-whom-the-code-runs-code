// Exercise A4.3 - The Bindery Shifts
// Session: A4, exercise 3 of 6 (6 hours for the evaluation)
// Theme: chapter 04 revision (binary search on the answer, with a
//   greedy feasibility check)
// Tags: chapter-04, binary-search, greedy
//
// MODEL: minimize the maximum block sum over partitions of a sequence
//   into k contiguous blocks. The decision version - "can the run be
//   bound in at most k shifts, none thicker than x?" - is monotone in
//   x: thicker allowances only help. Monotone decision + greedy check
//   = binary search on the answer, the chapter's signature move.
// MATH: feasible(x): scan left to right, packing signatures into the
//   current shift while the running sum stays <= x; on overflow, close
//   the shift and start another. Feasible iff shifts used <= k and no
//   single t_i exceeds x. Search the smallest feasible x in
//   [max t_i, sum t_i].
// PROOF: (greedy check is exact) the scan keeps every shift maximal;
//   exchange argument: any valid partition into at most k shifts can
//   be transformed shift by shift to the greedy one without increasing
//   the count, because the greedy first shift extends at least as far
//   as any valid first shift, and induction applies to the rest.
//   (binary search) feasibility is monotone, so the predicate is a
//   step function and the search converges to its threshold, which is
//   attained by the partition the final check constructs.
// COMPLEXITY: O(n log S) with S the thickness sum (~60 iterations of
//   an O(n) scan at these limits: ~1.2e7 operations). O(1) extra
//   memory.
// TYPES: sums reach 2e5 * 1e9 = 2e14: long long for the running sum
//   and both search bounds. The midpoint lo + (hi - lo) / 2 avoids
//   the textbook overflow even though long long has slack here -
//   habits are cheaper than audits.
// ALTERNATIVES: DP over (prefix, shifts) is O(n^2 k) - exact and dead
//   at n = 2e5; the parametric/"Aronszajn" style two-pointer
//   refinements save the log factor and cost clarity. Binary search
//   on the answer is the intended tool and the reason this exercise
//   sits in the revision slot.
// EDGE CASES: k = n (answer = max t_i); k = 1 (answer = sum); all
//   thicknesses equal; one giant signature dominating every shift;
//   k > number of signatures cannot happen (k <= n by contract).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o shifts a4-03-bindery-shifts.cpp

#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <vector>

namespace {

bool feasible(const std::vector<long long>& t, int k, long long cap) {
    int shifts = 1;
    long long load = 0;
    for (const long long x : t) {
        if (x > cap) return false;
        if (load + x <= cap) {
            load += x;
        } else {
            ++shifts;
            load = x;
        }
    }
    return shifts <= k;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<long long> t(static_cast<std::size_t>(n));
    for (auto& x : t) {
        std::cin >> x;
    }

    long long lo = *std::ranges::max_element(t);
    long long hi = std::accumulate(t.begin(), t.end(), 0LL);
    while (lo < hi) {
        const long long mid = lo + (hi - lo) / 2;
        if (feasible(t, k, mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    std::print("{}\n", lo);
    return 0;
}
