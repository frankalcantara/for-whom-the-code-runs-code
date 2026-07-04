// Exercise A7.9 - The Proofreader's Needle
// Session: A7, exercise 9 of 9
// Theme: chapter 12 (KMP prefix function)
// Tags: chapter-12, string-algorithms, kmp
//
// MODEL: count all occurrences of a pattern in a text, including
//   overlaps.
// MATH: the prefix function pi[i] is the length of the longest
//   proper prefix of the pattern that is also a suffix ending at i.
//   While scanning the text, the current matched length falls back
//   through pi when the next character does not match. A full match
//   increments the answer and falls back to pi[m-1], allowing
//   overlaps.
// PROOF: loop invariant. Before each text character is consumed,
//   matched is the longest prefix of the pattern that is a suffix of
//   the consumed text. The fallback chain preserves exactly the
//   candidate suffixes that could still match. Therefore every time
//   matched reaches m, one occurrence ends at the current position,
//   and no occurrence is missed or counted twice.
// COMPLEXITY: O(|pattern| + |text|) time and O(|pattern|) memory.
// TYPES: lengths up to 1e6 fit int. The occurrence count also fits
//   int, but long long is used for output.
// ALTERNATIVES: naive matching is O(nm); Z-function on pattern +
//   separator + text is equally valid, but KMP stores only pattern
//   state.
// EDGE CASES: pattern length 1; pattern equal to text; overlapping
//   occurrences such as ana in bananana; no occurrence.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o needle a7-09-proofreaders-needle.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feneedle.exe a7-09-proofreaders-needle.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace {

std::vector<int> prefix_function(const std::string& pattern) {
    std::vector<int> pi(pattern.size(), 0);
    for (std::size_t i = 1; i < pattern.size(); ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[static_cast<std::size_t>(j)]) {
            j = pi[static_cast<std::size_t>(j - 1)];
        }
        if (pattern[i] == pattern[static_cast<std::size_t>(j)]) ++j;
        pi[i] = j;
    }
    return pi;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;
    const auto pi = prefix_function(pattern);

    long long answer = 0;
    int matched = 0;
    for (const char ch : text) {
        while (matched > 0 &&
               ch != pattern[static_cast<std::size_t>(matched)]) {
            matched = pi[static_cast<std::size_t>(matched - 1)];
        }
        if (ch == pattern[static_cast<std::size_t>(matched)]) ++matched;
        if (matched == static_cast<int>(pattern.size())) {
            ++answer;
            matched = pi.back();
        }
    }

    std::print("{}\n", answer);
    return 0;
}
