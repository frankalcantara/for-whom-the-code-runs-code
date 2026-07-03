// Exercise T03.1 - The Telegraph Counter
// Session: T03.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 03 (input dominates; fread parser for 1e7 values)
// Tags: chapter-03
//
// MODEL: count indices i > 1 with s_i > s_{i-1} in a stream of n values.
// MATH: the comparison needs only the current and the previous value, so
//   the algorithm is a one-state scan: O(n) comparisons over up to 1e7
//   integers. At this size the bottleneck is not the comparison; it is the
//   path each byte takes from stdin to an int.
// PROOF: fold invariant. After consuming s_1..s_i, `rising` equals the
//   number of strict increases among consecutive pairs seen so far and
//   `previous` equals s_i. Each step compares exactly one new pair, so
//   induction on i gives the total.
// COMPLEXITY: O(n) time, O(1) memory beyond the input buffer.
// TYPES: strengths fit in int (|s| <= 1e9 < 2^31 - 1); the counter fits in
//   int (< n <= 1e7), kept as long long by the cheap-habit rule.
// ALTERNATIVES: default std::cin at n = 1e7 spends most of the time limit
//   on stream machinery; unsynced std::cin is usually enough; the fread
//   parser below removes the remaining per-token overhead and is the
//   Chapter 3 tool this unit trains. Values are never stored: a vector of
//   1e7 ints would cost 40 MB to remember numbers needed exactly once.
// EDGE CASES: n = 2; strictly decreasing log (answer 0); equal neighbors
//   (not a rise, comparison is strict); negative strengths; alternating
//   rise and fall.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o telegraph t03-s1-1-telegraph-counter.cpp

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
    long long rising = 0;
    for (long long i = 1; i < n; ++i) {
        long long current = fast::read_int();
        if (current > previous) ++rising;
        previous = current;
    }

    std::print("{}\n", rising);
    return 0;
}
