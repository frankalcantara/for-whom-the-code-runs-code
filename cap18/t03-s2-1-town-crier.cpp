// Exercise T03.2 - The Town Crier's Breath
// Session: T03.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 03 (string_view tokenization: parsing without copying)
// Tags: chapter-03
//
// MODEL: split one line into maximal space-free tokens; report the token
//   count and the first token of maximum length.
// MATH: a word is a maximal run of non-space characters. Scanning the line
//   once and classifying each character as inside or outside a run finds
//   every boundary; each character is examined exactly once.
// PROOF: run decomposition is unique: word starts are exactly the
//   non-space positions whose predecessor is a space or the line start.
//   The scan detects exactly these positions (count) and the matching run
//   ends (length). Keeping the champion only on a strictly greater length
//   preserves the first-longest tie rule, by induction over words.
// COMPLEXITY: O(L) time for a line of L characters; O(L) memory to own the
//   line once.
// TYPES: the line is owned by one std::string; every word is a
//   std::string_view into it, a pointer and a length, no allocation per
//   word. The views stay valid because the owning string outlives them and
//   is never modified after the views are taken. Copying each word into
//   its own string would allocate up to ~5e5 times for nothing.
// ALTERNATIVES: stream extraction (std::cin >> word) tokenizes too, but
//   each extraction copies characters into a fresh string buffer;
//   std::views::split (Chapter 2 ranges) expresses the same scan lazily.
//   The explicit two-pointer scan keeps the boundary logic visible, which
//   is the point of the hour.
// EDGE CASES: leading and trailing spaces; multiple spaces between words;
//   a single word; all words the same length (the first wins); a word
//   spanning the whole line.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o crier t03-s2-1-town-crier.cpp

#include <iostream>
#include <print>
#include <string>
#include <string_view>

int main() {
    std::string line;
    std::getline(std::cin, line);
    std::string_view text{line};

    long long words = 0;
    std::string_view longest{};

    std::size_t i = 0;
    while (i < text.size()) {
        while (i < text.size() && text[i] == ' ') ++i;
        if (i >= text.size()) break;
        std::size_t start = i;
        while (i < text.size() && text[i] != ' ') ++i;
        std::string_view word = text.substr(start, i - start);
        ++words;
        if (word.size() > longest.size()) longest = word;
    }

    std::print("{} {}\n", words, longest);
    return 0;
}
