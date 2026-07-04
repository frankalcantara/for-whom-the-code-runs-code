// Exercise T12.1 - The Lighthouse Logbook
// Session: T12.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 12 (prefix function, KMP matching)
// Tags: chapter-12, string-algorithms, kmp, brute-force
//
// MODEL: count occurrences (overlapping) of pattern w in text s.
// MATH: prefix function pi[i] of w = length of the longest proper
//   border of w[0..i]. KMP runs the text through the pattern
//   automaton: keep the length q of the longest prefix of w that is
//   a suffix of the text read so far; on each character, fall back
//   through borders (q = pi[q-1]) until the next character extends,
//   then extend. q = |w| means an occurrence ends here; fall back
//   to pi[|w|-1] and keep going - that is what permits overlaps.
// PROOF: invariant - after reading s[0..i], q is the length of the
//   LONGEST prefix of w ending at i. The fallback chain enumerates
//   ALL borders in decreasing length (chapter 12 lemma), so the
//   first extendable one is the longest extension: the invariant
//   holds by induction, and every occurrence is seen exactly once
//   (an occurrence ending at i IS a prefix of w of length |w|
//   ending at i, and q is maximal).
// COMPLEXITY: O(|s| + |w|) time - q only grows |s| times in total
//   and each fallback strictly decreases it, so the falls are
//   amortized free. O(|w|) extra memory; the text could even be
//   streamed.
// TYPES: lengths <= 1e6 fit int; the count can reach |s| - |w| + 1
//   ~ 1e6, also int, but long long costs nothing and removes the
//   thought. Indices as std::size_t to match std::string.
// ALTERNATIVES: Z-function on w#s - same bound, second array of
//   size |s|; std::string::find in a loop - correct with pos+1
//   stepping but worst-case quadratic on adversarial inputs
//   (implementation-defined); hashing - probabilistic, needs two
//   mods to defend, more code for no gain here.
// EDGE CASES: w = s (1); |w| = 1 (counts a letter); self-overlap
//   chains (the example, and aaaa/aa -> 3); w absent (0); w longer
//   than s excluded by constraints.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o logbook t12-s1-1-lighthouse-logbook.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Felogbook.exe t12-s1-1-lighthouse-logbook.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace {

std::vector<std::size_t> prefix_function(const std::string& w) {
    std::vector<std::size_t> pi(w.size(), 0);
    for (std::size_t i = 1; i < w.size(); ++i) {
        std::size_t q = pi[i - 1];
        while (q > 0 && w[i] != w[q]) q = pi[q - 1];
        if (w[i] == w[q]) ++q;
        pi[i] = q;
    }
    return pi;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::string w;
    std::cin >> s >> w;

    const std::vector<std::size_t> pi = prefix_function(w);
    long long count = 0;
    std::size_t q = 0;
    for (const char ch : s) {
        while (q > 0 && ch != w[q]) q = pi[q - 1];
        if (ch == w[q]) ++q;
        if (q == w.size()) {
            ++count;
            q = pi[q - 1];  // continue: overlaps allowed
        }
    }
    std::print("{}\n", count);
    return 0;
}
