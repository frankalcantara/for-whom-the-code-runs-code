// Exercise T03.5 - The Loom Inspector
// Session: T03.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 03 (streaming scan at 1e7 scale; state instead of storage)
// Tags: chapter-03
//
// MODEL: longest run of equal consecutive values in a stream of n colors.
// MATH: a run is a maximal block of equal neighbors. The scan needs three
//   facts at any moment: the previous color, the length of the run ending
//   here, and the best run length so far. Nothing else about the past can
//   influence the future, so nothing else is stored.
// PROOF: loop invariant. After reading color c_i, `current` is the length
//   of the maximal equal run ending at position i and `best` is the
//   maximum run length among positions 1..i. The transition (extend on
//   equal, reset to 1 on different) preserves both claims; induction on i.
// COMPLEXITY: O(n) time, O(1) memory: the bolt is never stored. A vector
//   of 1e7 colors would spend 40 MB remembering stripes the inspector has
//   already walked past.
// TYPES: colors fit in int; run lengths fit in int (<= 1e7), held in
//   long long by the standing habit.
// ALTERNATIVES: reading with unsynced std::cin works at this size on most
//   judges; the fread parser keeps the program's fate independent of the
//   stream library's mood. Storing the bolt and scanning twice is the
//   memory-shaped version of forgetting that one pass suffices.
// EDGE CASES: n = 1 (answer 1); all stripes equal (answer n); all stripes
//   distinct (answer 1); the longest run at the very end of the bolt (the
//   final comparison must not be skipped).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o loom t03-s4-1-loom-inspector.cpp

#include <cstdio>
#include <print>

namespace fast {
constexpr int BUF_SIZE = 1 << 22;
char buf[BUF_SIZE];
int pos = 0;
int len = 0;

char next_char() {
    if (pos == len) {
        len = static_cast<int>(std::fread(buf, 1, BUF_SIZE, stdin));
        pos = 0;
        if (len == 0) return '\0';
    }
    return buf[pos++];
}

long long read_int() {
    char c = next_char();
    while (c != '-' && (c < '0' || c > '9') && c != '\0') c = next_char();
    long long sign = 1;
    if (c == '-') { sign = -1; c = next_char(); }
    long long value = 0;
    while (c >= '0' && c <= '9') {
        value = value * 10 + (c - '0');
        c = next_char();
    }
    return sign * value;
}
}  // namespace fast

int main() {
    long long n = fast::read_int();

    long long previous = fast::read_int();
    long long current = 1;
    long long best = 1;

    for (long long i = 1; i < n; ++i) {
        long long color = fast::read_int();
        current = (color == previous) ? current + 1 : 1;
        if (current > best) best = current;
        previous = color;
    }

    std::print("{}\n", best);
    return 0;
}
