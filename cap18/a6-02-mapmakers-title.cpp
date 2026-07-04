// Exercise A6.2 - The Mapmaker's Title
// Session: A6, exercise 2 of 8 (6 hours for the set)
// Theme: chapter 12 (Z-function)
// Tags: chapter-12, sliding-window, string-algorithms, z-function
//
// MODEL: longest prefix of s occurring at a position other than 0.
// MATH: the Z-function gives z[i] = length of the longest common
//   prefix of s and s[i..]. A prefix of length L occurs at
//   position i > 0 iff z[i] >= L, so the answer is max_{i>0} z[i].
//   The Z computation keeps the rightmost match window [l, r):
//   inside it, z[i] starts from min(z[i - l], r - i) (the prefix
//   already matched tells us that much); extension proceeds by
//   direct comparison and moves r only forward.
// PROOF: correctness of the seed value: s[i..r) equals
//   s[i-l..r-l) because [l, r) matches a prefix, so the first
//   min(z[i-l], r-i) characters of s[i..] match the prefix without
//   looking - and no more can be claimed since either z[i-l] is
//   exact or the window ends. Each character comparison that
//   succeeds advances r, which never retreats: O(n) total work.
//   The max over i > 0 is the answer by the iff above.
// COMPLEXITY: O(n) time, O(n) memory.
// TYPES: lengths <= 1e6 in std::size_t; no arithmetic risk.
// ALTERNATIVES: prefix function - max over the pi array catches
//   borders (prefix occurrences that END the string) but misses
//   internal occurrences unless one takes max over all pi values,
//   which works too and is the classic duality; hashing with
//   binary search on L - O(n log n), probabilistic; suffix array -
//   heavy machinery for one question.
// EDGE CASES: all letters distinct (0); all letters equal (n-1,
//   from position 1); answer length limited by position (suffix
//   shorter than prefix); |s| = 1 (0); the prefix occurring only
//   as a border (e.g. abca... still counted - "elsewhere" includes
//   the end).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o title a6-02-mapmakers-title.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetitle.exe a6-02-mapmakers-title.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    const std::size_t n = s.size();
    std::vector<std::size_t> z(n, 0);
    std::size_t best = 0;
    std::size_t l = 0;
    std::size_t r = 0;  // window [l, r) matches a prefix
    for (std::size_t i = 1; i < n; ++i) {
        if (i < r) z[i] = std::min(z[i - l], r - i);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
        best = std::max(best, z[i]);
    }
    std::print("{}\n", best);
    return 0;
}
