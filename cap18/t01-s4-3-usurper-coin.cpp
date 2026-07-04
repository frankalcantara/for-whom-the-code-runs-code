// Exercise T01.7 - The Usurper's Coin
// Session: T01.S4, exercise 3 of 3 (2 hours for the three; this one resists)
// Theme: chapter 01 (an invariant derived from scratch; O(n) time, O(1) state)
// Tags: chapter-01
//
// MODEL: given n marks, find the mark with strictly more than n/2
//   occurrences, or report NONE. (Majority element.)
// MATH: imagine repeatedly removing PAIRS of coins with DIFFERENT marks.
//   Each removal takes at most one coin away from any single mark, and
//   takes two coins away from the chest. A mark holding more than half of
//   the chest loses coins more slowly than the chest shrinks, so it can
//   never be eliminated: when no mixed pair remains, the survivors all
//   carry the majority mark, if one exists.
// PROOF: cancellation invariant. Maintain a candidate mark and a weight w,
//   meaning "the coins seen so far cancel into w unpaired coins, all of
//   the candidate's mark". A matching coin sets w <- w + 1; a different
//   coin cancels one survivor, w <- w - 1; at w = 0 the next coin starts a
//   new pile. Every decrement destroys one candidate coin and one other
//   coin, which is exactly a mixed-pair removal. By the counting argument
//   above, a true majority mark ends with w > 0 and is the final candidate.
//   The converse fails (a candidate may be a mere plurality), so a second
//   pass counts the candidate's occurrences and applies the strict test.
// COMPLEXITY: two passes, O(n) time; O(1) state beyond the stored coins.
// TYPES: n <= 1e6 and marks <= 1e9 fit in int; the count fits in int; the
//   strict comparison 2 * count > n is done in long long out of habit, the
//   cheap kind of habit that survives constraint changes.
// ALTERNATIVES: sort and inspect the middle element, O(n log n), correct
//   because a majority must occupy the median position; a frequency table
//   needs hashing (Chapter 6 vocabulary) or 1e9 counters (a confession).
//   The cancellation argument needs neither.
// EDGE CASES: n = 1 (that coin is a majority); exactly half (NONE, the
//   test is strict); majority by one coin; all coins identical.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o usurper t01-s4-3-usurper-coin.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feusurper.exe t01-s4-3-usurper-coin.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> mark(n);
    for (auto& x : mark) std::cin >> x;

    // Pass 1: cancellation. Find the only possible majority candidate.
    int candidate = mark[0];
    int weight = 0;
    for (int x : mark) {
        if (weight == 0) { candidate = x; weight = 1; }
        else if (x == candidate) ++weight;
        else --weight;
    }

    // Pass 2: the invariant only proves the candidate CAN be the majority.
    long long count = 0;
    for (int x : mark)
        if (x == candidate) ++count;

    if (2 * count > n) std::print("{}\n", candidate);
    else               std::print("NONE\n");
    return 0;
}
