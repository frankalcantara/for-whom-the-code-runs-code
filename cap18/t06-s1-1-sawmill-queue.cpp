// Exercise T06.1 - The Sawmill Queue
// Session: T06.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 06 (binary heap; priority_queue as the maintained maximum)
// Tags: chapter-06
//
// MODEL: a dynamic multiset under two operations, insert(g) and
//   extract-max. The priority queue is the exact contract: it promises
//   the maximum, and only the maximum, cheaper than full order.
// MATH: a binary heap stores the multiset in an array with the heap
//   invariant a[parent] >= a[child]. Insert appends and sifts up;
//   extract-max swaps the root with the last leaf and sifts down. Both
//   walks follow one root-leaf path of length floor(log2(size)).
// PROOF: the invariant guarantees the root is a maximum, by induction on
//   the tree height (each node dominates its subtree, because >= is
//   transitive along paths). Sift-up and sift-down each restore the
//   invariant by fixing the only violated edge and pushing the violation
//   one level until it vanishes; termination is the finite path length.
// COMPLEXITY: O(log n) per event, O(q log q) total, O(q) memory. A
//   sorted vector pays O(n) per insert; re-sorting per cut pays
//   O(n log n) per event; a multiset matches O(log n) but with worse
//   constants (pointers, allocation) for strictly less required power.
// TYPES: girths fit in int (1e9 < 2^31 - 1); the count of events fits
//   in int. Nothing here grows: no sums, no products, no overflow story,
//   which is itself worth noticing once per training program.
// ALTERNATIVES: std::multiset with prev(end()) gives the same bounds and
//   also supports deleting arbitrary logs (not asked); a Fenwick tree
//   over compressed girths answers order statistics (not asked). The heap
//   is the smallest tool that signs this particular contract.
// EDGE CASES: cut immediately after a single arrival; repeated equal
//   girths (any of them may be taken, the printed value is the same);
//   alternating arrive/cut down to an empty yard; all arrivals first,
//   all cuts after (heap drains in sorted order).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o sawmill t06-s1-1-sawmill-queue.cpp

#include <iostream>
#include <print>
#include <queue>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q = 0;
    std::cin >> q;

    std::priority_queue<int> yard;
    while (q-- > 0) {
        int type = 0;
        std::cin >> type;
        if (type == 1) {
            int g = 0;
            std::cin >> g;
            yard.push(g);
        } else {
            std::print("{}\n", yard.top());
            yard.pop();
        }
    }
    return 0;
}
