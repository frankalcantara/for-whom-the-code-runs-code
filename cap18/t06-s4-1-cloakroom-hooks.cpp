// Exercise T06.5 - The Cloakroom Hooks
// Session: T06.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 06 (std::set as an ordered pool; smallest free element
//   under interleaved take and return)
// Tags: chapter-06, sorting
//
// MODEL: a dynamic set of free hooks under three primitives: minimum,
//   erase-minimum, insert. An ordered set signs exactly this contract.
// MATH: a red-black tree keeps the free hooks sorted; begin() is the
//   minimum, erase and insert rebalance locally. All three primitives
//   are O(log n) with begin() amortized O(1) in libstdc++ (the leftmost
//   pointer is cached).
// PROOF: the invariant is "the set contains exactly the free hooks".
//   Arrival removes the minimum (the smallest free hook, by the order
//   property of the tree); departure inserts a hook that was occupied,
//   so no duplicate can occur and the statement's guarantee (a free hook
//   exists on arrival) keeps begin() well defined. Correctness of the
//   tree operations is the container's contract, not this program's job.
// COMPLEXITY: O(q log n) time, O(n) memory. A boolean array with a
//   linear scan for the first free hook is O(nq) = 4e10 in the
//   adversarial dance around the same low hooks; a min-heap of free
//   hooks works only with lazy deletion baggage, since departures
//   reinsert hooks the heap may still hold stale copies of.
// TYPES: hook numbers and counters fit in int; nothing accumulates.
// ALTERNATIVES: a heap plus an occupied[] bitmap with lazy pops (same
//   bound, more invariants to keep in the head); a Fenwick tree over
//   0/1 free flags with binary descent finds the first free hook in
//   O(log n), the chapter's order-statistics trick, justified only when
//   "smallest free" mutates under range operations. The set is the
//   smallest honest tool.
// EDGE CASES: all hooks taken then one freed (the freed hook is the
//   next assigned); departures in arbitrary order; n = 1 cycling
//   take/free; assignment after the pool refills past previous holes.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o cloakroom t06-s4-1-cloakroom-hooks.cpp

#include <iostream>
#include <numeric>
#include <print>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::set<int> free_hooks;
    for (int h = 1; h <= n; ++h) free_hooks.insert(free_hooks.end(), h);

    while (q-- > 0) {
        int type = 0;
        std::cin >> type;
        if (type == 1) {
            auto it = free_hooks.begin();
            std::print("{}\n", *it);
            free_hooks.erase(it);
        } else {
            int h = 0;
            std::cin >> h;
            free_hooks.insert(h);
        }
    }
    return 0;
}
