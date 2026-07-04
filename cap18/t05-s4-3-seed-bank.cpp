// Exercise T05.7 - The Seed Bank
// Session: T05.S4, exercise 3 of 3 (2 hours for the three; the hard one)
// Theme: chapters 04-05 (Mo's algorithm; sorting queries, coordinate
//   compression via sort + unique from chapter 04)
// Tags: chapter-04, chapter-05, sorting, sliding-window, mo-algorithm, number-theory
//
// MODEL: offline distinct-count over ranges. The statement legalizes the
//   reordering ("may answer them in any order"), which is Mo's license.
// MATH: maintain a frequency table over a current window [L, R] and the
//   count of values with freq > 0. Moving one endpoint by one shelf is
//   O(1): freq going 0 -> 1 adds a distinct variety, 1 -> 0 removes one.
//   Sort queries by (block of L, R) with R alternating direction per
//   block (the parity trick); blocks have size ~ n / sqrt(q).
// PROOF: correctness is immediate (the window always matches the query
//   being answered; add/remove are exact inverses). The cost bound: L
//   moves O(block) per query, total O(q * n / sqrt(q)) = O(n sqrt q);
//   R sweeps each block's query list monotonically, O(n) per block, and
//   there are ~ sqrt(q) blocks: O(n sqrt q) total moves overall.
// COMPLEXITY: O((n + q) sqrt q) pointer moves ~ 2e5 * 450 ~ 9e7 O(1)
//   steps, plus O(n log n + q log q) for the two sorts. Memory O(n + q).
// TYPES: codes reach 1e9, so the frequency table cannot be indexed by
//   value; sort + unique + lower_bound (chapter 04) compresses codes to
//   [0, n). Counts fit in int (a window holds at most n = 2e5 shelves).
// ALTERNATIVES: the offline BIT solution (sort queries by R, Fenwick over
//   last occurrence) is O((n + q) log n) and strictly faster, but Fenwick
//   is chapter 06 material, not yet liberado; persistent structures are
//   further still. Online answering would force them; this statement
//   deliberately does not. unordered_map instead of compression works but
//   pays hashing on every move of the hot loop.
// EDGE CASES: l = r (one shelf, answer 1); all shelves the same variety;
//   all distinct; repeated identical queries (the window does not move);
//   queries covering the whole row; n = 1.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o seedbank t05-s4-3-seed-bank.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feseedbank.exe t05-s4-3-seed-bank.cpp

#include <algorithm>
#include <cmath>
#include <iostream>
#include <print>
#include <vector>

struct Query {
    int l, r, idx;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<int> v(n);
    for (auto& x : v) std::cin >> x;

    // coordinate compression (chapter 04: sort, unique, lower_bound)
    std::vector<int> sorted = v;
    std::ranges::sort(sorted);
    auto dup = std::ranges::unique(sorted);
    sorted.erase(dup.begin(), dup.end());
    for (auto& x : v) {
        x = static_cast<int>(std::ranges::lower_bound(sorted, x) -
                             sorted.begin());
    }

    std::vector<Query> qs(q);
    for (int i = 0; i < q; ++i) {
        std::cin >> qs[i].l >> qs[i].r;
        --qs[i].l;
        --qs[i].r;
        qs[i].idx = i;
    }

    int block = std::max(1, static_cast<int>(n / std::sqrt(double(q) + 1.0)));
    std::ranges::sort(qs, [block](const Query& a, const Query& b) {
        int ba = a.l / block;
        int bb = b.l / block;
        if (ba != bb) return ba < bb;
        return (ba & 1) ? a.r > b.r : a.r < b.r;  // parity trick
    });

    std::vector<int> freq(sorted.size(), 0);
    std::vector<int> answer(q, 0);
    int distinct = 0;
    auto add = [&](int i) { distinct += (freq[v[i]]++ == 0); };
    auto remove = [&](int i) { distinct -= (--freq[v[i]] == 0); };

    int curl = 0;
    int curr = -1;
    for (const auto& query : qs) {
        while (curr < query.r) add(++curr);
        while (curl > query.l) add(--curl);
        while (curr > query.r) remove(curr--);
        while (curl < query.l) remove(curl++);
        answer[query.idx] = distinct;
    }

    for (int i = 0; i < q; ++i) std::print("{}\n", answer[i]);
    return 0;
}
