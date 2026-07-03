// Exercise T07.6 - The Milk Churns
// Session: T07.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 07 (BFS over an abstract state space)
// Tags: chapter-07, bfs, number-theory, gcd
//
// MODEL: the state is the pair (x, y) of current contents,
//   0 <= x <= a, 0 <= y <= b. The six moves (fill x, fill y, empty x,
//   empty y, pour x->y, pour y->x) are the edges, each of cost 1. The
//   answer is the BFS distance from (0, 0) to the nearest state with
//   x = c or y = c.
// MATH: pouring x into y moves t = min(x, b - y) litres: the source
//   loses t, the destination gains t. The state space has
//   (a + 1)(b + 1) <= 1,002,001 states and 6 edges per state. BFS
//   explores it level by level, so the first goal state found is
//   reached in the minimum number of moves.
// PROOF: BFS optimality on unweighted graphs (induction on distance:
//   when a state is dequeued, its recorded distance is final, because
//   every shorter path would have enqueued it earlier). Reachability
//   side note: every reachable amount is a multiple of gcd(a, b) no
//   greater than max(a, b) - Bezout - so `impossible` is decidable by
//   arithmetic; the BFS discovers the same fact by exhaustion, which
//   is the chapter's honest, theorem-free route.
// COMPLEXITY: O(ab) states, O(ab) time and memory; at a = b = 1000
//   about 1e6 states and 6e6 edge relaxations - well within limits.
// TYPES: contents fit in int; states are encoded as x * (b + 1) + y in
//   an int, max ~1e6, no overflow. Distances fit in int (a path never
//   needs more states than exist).
// ALTERNATIVES: the extended Euclidean algorithm produces SOME pouring
//   sequence and a reachability test in O(log min(a,b)), but not the
//   minimum number of moves (Chapter 10 machinery anyway, not yet
//   available at T07). DFS finds a path, not a shortest one. Bidirectional
//   BFS halves the frontier; unnecessary at this size.
// EDGE CASES: c = 0 (answer 0: dawn already qualifies); c = a or c = b
//   (answer 1); c > max(a, b) (impossible: no churn can hold it);
//   gcd-unreachable c (e.g. a = 6, b = 4, c = 3); a = b with c not in
//   {0, a} (impossible); a = 1 (every c <= 1 reachable fast).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o churns t07-s4-2-milk-churns.cpp

#include <iostream>
#include <print>
#include <queue>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int a = 0;
    int b = 0;
    int c = 0;
    std::cin >> a >> b >> c;

    const int width = b + 1;
    auto encode = [width](int x, int y) { return x * width + y; };

    std::vector<int> dist(static_cast<std::size_t>((a + 1) * (b + 1)), -1);
    std::queue<int> frontier;
    dist[static_cast<std::size_t>(encode(0, 0))] = 0;
    frontier.push(encode(0, 0));

    int answer = (c == 0) ? 0 : -1;
    while (!frontier.empty() && answer == -1) {
        const int id = frontier.front();
        frontier.pop();
        const int x = id / width;
        const int y = id % width;

        const int pour_xy = std::min(x, b - y);
        const int pour_yx = std::min(y, a - x);
        const int next_states[6][2] = {
            {a, y},                          // fill x
            {x, b},                          // fill y
            {0, y},                          // empty x
            {x, 0},                          // empty y
            {x - pour_xy, y + pour_xy},      // pour x -> y
            {x + pour_yx, y - pour_yx},      // pour y -> x
        };
        for (const auto& s : next_states) {
            const int nid = encode(s[0], s[1]);
            if (dist[static_cast<std::size_t>(nid)] != -1) continue;
            dist[static_cast<std::size_t>(nid)] = dist[static_cast<std::size_t>(id)] + 1;
            if (s[0] == c || s[1] == c) {
                answer = dist[static_cast<std::size_t>(nid)];
                break;
            }
            frontier.push(nid);
        }
    }

    if (answer == -1) {
        std::print("impossible\n");
    } else {
        std::print("{}\n", answer);
    }
    return 0;
}
