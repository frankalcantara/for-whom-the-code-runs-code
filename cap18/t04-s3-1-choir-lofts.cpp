// Exercise T04.3 - The Choir Lofts
// Session: T04.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 04 (counting sort: a known tiny domain beats comparisons)
// Tags: chapter-04, sorting
//
// MODEL: sort n integers drawn from the fixed domain [50, 250].
// MATH: the domain has 201 possible values. Counting how many singers
//   have each height, then replaying the counts in increasing height,
//   produces sorted output without ever comparing two singers. The
//   information that comparison sorts must buy with Omega(n log n)
//   comparisons is already in the statement: the domain.
// PROOF: the output lists each height h exactly count[h] times, in
//   increasing h, so it is a non-decreasing rearrangement of the input
//   multiset; that is the definition of the sorted sequence. (Stability
//   is irrelevant here: equal heights are indistinguishable integers.)
// COMPLEXITY: O(n + 201) time, O(201) counters beyond input and output.
//   For n = 1e6, std::ranges::sort pays ~2e7 comparisons; the counters pay
//   one array pass.
// TYPES: heights fit in int; counters reach n = 1e6, also int; the output
//   is the volume risk (up to 1e6 numbers), so it is assembled in one
//   buffer and written once, the Chapter 3 lesson holding the door.
// ALTERNATIVES: std::ranges::sort is correct, shorter, and at this n
//   would also pass; the exercise is recognizing when the domain makes
//   comparisons unnecessary. A radix sort is the same idea applied when
//   the domain is large but the key width is fixed.
// EDGE CASES: all singers the same height; heights at both domain ends
//   (50 and 250 must index correctly); n = 1; an input already sorted
//   (no special case, the counts do not care).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o choir t04-s3-1-choir-lofts.cpp

#include <array>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::array<int, 251> count{};
    for (int i = 0; i < n; ++i) {
        int h = 0;
        std::cin >> h;
        ++count[h];
    }

    std::string out;
    out.reserve(static_cast<std::size_t>(n) * 4);
    bool first = true;
    for (int h = 50; h <= 250; ++h)
        for (int c = 0; c < count[h]; ++c) {
            if (!first) out.push_back(' ');
            out += std::to_string(h);
            first = false;
        }
    out.push_back('\n');

    std::print("{}", out);
    return 0;
}
