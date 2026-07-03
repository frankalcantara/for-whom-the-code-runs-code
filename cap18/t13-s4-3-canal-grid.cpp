// Exercise T13.7 - The Canal Grid
// Session: T13.S4, exercise 3 of 3 (2 hours for all three; this is
//   the integrator)
// Theme: chapters 13 + 06 + 04, integration (sweep line over
//   events; Fenwick tree for range counting; sort with a
//   tie-breaking discipline that IS the correctness argument)
// Tags: chapter-13, chapter-06, chapter-04, sorting, fenwick-tree, geometry, sweep-line
//
// MODEL: count intersecting (horizontal, vertical) pairs of
//   axis-aligned segments, endpoints touching included.
// MATH: sweep a vertical line left to right. A horizontal canal
//   [x1, x2] at height y is ALIVE while the sweep is inside
//   [x1, x2]: it enters the structure at x1 and leaves after x2.
//   A vertical canal at x spanning [y1, y2] asks: how many alive
//   horizontals have y in [y1, y2]? - a range count over y,
//   answered by a Fenwick tree on COMPRESSED horizontal heights.
//   Endpoint touches dictate the tie order at equal x:
//   additions first, then queries, then removals - a vertical at a
//   horizontal's left OR right endpoint must see it alive.
// PROOF: a pair intersects iff the vertical's x lies in the
//   horizontal's [x1, x2] and the horizontal's y lies in the
//   vertical's [y1, y2] (axis-aligned geometry reduces to interval
//   membership in each axis). The first condition is exactly
//   "alive at the query, under add-query-remove tie order"; the
//   second is exactly the Fenwick range count. Each pair is
//   counted at one event - the vertical's - once.
// COMPLEXITY: O(n log n): sort of 3 events per canal at most,
//   Fenwick operations O(log n). Memory O(n).
// TYPES: the answer reaches ~1e10 (a grid of 1e5 by 1e5 canals):
//   long long. Coordinates only feed comparisons and compression.
//   Fenwick cells hold counts <= n: int32.
// ALTERNATIVES: for each vertical, binary search a static sorted
//   structure - but the alive-set CHANGES along x, which is the
//   whole problem; an interval tree keyed by x intervals - works,
//   heavier; O(n^2) pair checks - 4e10, dead. The sweep converts
//   a 2D problem into n dynamic 1D queries: chapter 13's central
//   trick.
// EDGE CASES: vertical exactly at x1 or x2 of a horizontal
//   (counts - the tie order exists for this); vertical's span
//   touching the horizontal's y at its boundary (inclusive range
//   count); disjoint everything (0); all horizontals at one
//   height crossed by one vertical (n - 1 in one query); negative
//   coordinates throughout (compression is sign-blind).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o canals t13-s4-3-canal-grid.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

class Fenwick {
   public:
    explicit Fenwick(std::size_t n) : tree_(n + 1, 0) {}

    void add(std::size_t i, int delta) {  // 1-based
        for (; i < tree_.size(); i += i & (~i + 1)) {
            tree_[i] += static_cast<std::int32_t>(delta);
        }
    }

    long long prefix(std::size_t i) const {
        long long s = 0;
        for (; i > 0; i -= i & (~i + 1)) s += tree_[i];
        return s;
    }

   private:
    std::vector<std::int32_t> tree_;
};

struct Event {
    long long x;
    int type;       // 0 = add, 1 = query, 2 = remove
    long long a;    // add/remove: y; query: y1
    long long b;    // query: y2
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;

    std::vector<Event> events;
    events.reserve(3 * n);
    std::vector<long long> heights;
    for (std::size_t i = 0; i < n; ++i) {
        long long x1 = 0;
        long long y1 = 0;
        long long x2 = 0;
        long long y2 = 0;
        std::cin >> x1 >> y1 >> x2 >> y2;
        if (y1 == y2) {  // horizontal
            if (x1 > x2) std::swap(x1, x2);
            events.push_back({x1, 0, y1, 0});
            events.push_back({x2, 2, y1, 0});
            heights.push_back(y1);
        } else {  // vertical
            if (y1 > y2) std::swap(y1, y2);
            events.push_back({x1, 1, y1, y2});
        }
    }

    std::sort(heights.begin(), heights.end());
    heights.erase(std::unique(heights.begin(), heights.end()), heights.end());
    const auto rank = [&](long long y) {  // 1-based
        return static_cast<std::size_t>(
                   std::lower_bound(heights.begin(), heights.end(), y) -
                   heights.begin()) +
               1;
    };

    std::sort(events.begin(), events.end(), [](const Event& l, const Event& r) {
        if (l.x != r.x) return l.x < r.x;
        return l.type < r.type;  // add, then query, then remove
    });

    Fenwick tree(heights.size());
    long long gates = 0;
    for (const Event& e : events) {
        if (e.type == 0) {
            tree.add(rank(e.a), +1);
        } else if (e.type == 2) {
            tree.add(rank(e.a), -1);
        } else {
            // count alive heights in [e.a, e.b]
            const std::size_t lo = static_cast<std::size_t>(
                std::lower_bound(heights.begin(), heights.end(), e.a) -
                heights.begin());  // 0-based index of first >= y1
            const std::size_t hi = static_cast<std::size_t>(
                std::upper_bound(heights.begin(), heights.end(), e.b) -
                heights.begin());  // 0-based index past last <= y2
            if (lo < hi) gates += tree.prefix(hi) - tree.prefix(lo);
        }
    }
    std::print("{}\n", gates);
    return 0;
}
