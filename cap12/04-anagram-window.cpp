// cap12/04-anagram-window.cpp
// Solved exercise: anagram counting and minimum covering window.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o anagram_window 04-anagram-window.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feanagram_window.exe 04-anagram-window.cpp

#include <array>
#include <cassert>
#include <climits>
#include <print>
#include <string>
#include <utility>

using Freq = std::array<int, 26>;

int count_anagrams(const std::string& txt, const std::string& pat) {
    int n = static_cast<int>(txt.size());
    int m = static_cast<int>(pat.size());
    if (m == 0 || m > n) return 0;

    Freq need{}, have{};
    for (char c : pat) ++need[c - 'a'];

    int diff = 0;
    for (int i = 0; i < 26; ++i)
        if (need[i] != 0) ++diff;

    int count = 0;
    for (int i = 0; i < n; ++i) {
        int in = txt[i] - 'a';
        ++have[in];
        if (have[in] == need[in]) --diff;
        else if (have[in] == need[in] + 1) ++diff;

        if (i >= m) {
            int out = txt[i - m] - 'a';
            if (have[out] == need[out]) ++diff;
            else if (have[out] == need[out] + 1) --diff;
            --have[out];
        }

        if (i >= m - 1 && diff == 0) ++count;
    }
    return count;
}

std::pair<int, int> min_window(const std::string& txt, const std::string& pat) {
    if (pat.empty()) return {-1, -1};

    int n = static_cast<int>(txt.size());
    Freq need{}, have{};
    for (char c : pat) ++need[c - 'a'];

    int required = 0;
    for (int i = 0; i < 26; ++i)
        if (need[i] > 0) ++required;

    int formed = 0, best_len = INT_MAX, best_start = -1;
    int left = 0;
    for (int right = 0; right < n; ++right) {
        int c = txt[right] - 'a';
        ++have[c];
        if (have[c] == need[c]) ++formed;

        while (formed == required) {
            if (right - left + 1 < best_len) {
                best_len = right - left + 1;
                best_start = left;
            }
            int lc = txt[left] - 'a';
            --have[lc];
            if (have[lc] < need[lc]) --formed;
            ++left;
        }
    }

    return (best_start == -1) ? std::make_pair(-1, -1)
                              : std::make_pair(best_start, best_len);
}

int main() {
    assert(count_anagrams("cbaebabacd", "abc") == 2);
    assert(count_anagrams("abab", "ab") == 3);
    assert(count_anagrams("a", "ab") == 0);
    assert(count_anagrams("hello", "xyz") == 0);
    assert(count_anagrams("abc", "") == 0);

    auto [s1, l1] = min_window("adobecodebanc", "abc");
    assert(s1 == 9 && l1 == 4);

    auto [s2, l2] = min_window("a", "a");
    assert(s2 == 0 && l2 == 1);

    auto [s3, l3] = min_window("abc", "d");
    assert(s3 == -1 && l3 == -1);

    std::print("04-anagram-window: all assertions passed\n");
    return 0;
}
