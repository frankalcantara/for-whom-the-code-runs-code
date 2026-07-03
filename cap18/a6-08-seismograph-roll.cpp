// Exercise A6.8 - The Seismograph Roll
// Session: A6, exercise 8 of 8 (6 hours for the set)
// Theme: chapter 03 revision (buffered fread parsing at 1e7 scale;
//   the computation is deliberately trivial)
// Tags: chapter-03
//
// MODEL: filtered count and sum over a stream - the algorithm is
//   one comparison per element; the exercise is moving 100+ MB of
//   text through the parser without the I/O eating the time limit.
// MATH: none worth the name: count += (a >= t), sum += a when it
//   qualifies. The point is the plumbing.
// PROOF: the parser reads digits into an accumulator between
//   non-digit separators; every integer in the stream is delimited
//   by whitespace per the contract, so each is reconstructed
//   exactly once (base-10 positional accumulation). The filter is
//   a direct transcription of the statement.
// COMPLEXITY: O(input bytes) with a constant near one table lookup
//   per byte. Memory: one fixed 1 MB buffer - the roll is never
//   resident, only streamed.
// TYPES: amplitudes <= 1e9 fit uint32/long long; the SUM reaches
//   1e7 * 1e9 = 1e16: long long, the classic accumulation
//   overflow. n itself fits int but is read as long long for
//   uniformity. The parser builds values in long long directly -
//   building in int and widening after would already have
//   overflowed single values past 2^31.
// ALTERNATIVES: cin with sync_with_stdio(false) - typically 2-4x
//   slower than the fread parser at this scale, often still inside
//   generous limits, and the first thing to try in a contest;
//   scanf - in between; mmap (chapter 3's appendix) - fastest cold
//   path, platform-bound. The fread parser is the portable fast
//   answer.
// EDGE CASES: t = 0 (everything counts: count = n, the 1e16 sum);
//   t greater than every amplitude (0 0); a_i = t exactly
//   (inclusive: counts); n = 1; amplitudes of differing digit
//   lengths back to back (parser state machine, not column
//   guessing).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o roll a6-08-seismograph-roll.cpp

#include <cstdio>
#include <print>

namespace {

constexpr std::size_t kBufferSize = 1 << 20;
char buffer[kBufferSize];
std::size_t buffer_len = 0;
std::size_t buffer_pos = 0;

char next_char() {
    if (buffer_pos == buffer_len) {
        buffer_len = std::fread(buffer, 1, kBufferSize, stdin);
        buffer_pos = 0;
        if (buffer_len == 0) return '\0';  // end of stream
    }
    return buffer[buffer_pos++];
}

long long read_int() {
    char c = next_char();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = next_char();
    long long value = 0;
    while (c >= '0' && c <= '9') {
        value = value * 10 + (c - '0');
        c = next_char();
    }
    return value;
}

}  // namespace

int main() {
    const long long n = read_int();
    const long long t = read_int();

    long long count = 0;
    long long sum = 0;
    for (long long i = 0; i < n; ++i) {
        const long long a = read_int();
        if (a >= t) {
            ++count;
            sum += a;
        }
    }
    std::print("{} {}\n", count, sum);
    return 0;
}
