// Exercise A3.4 - The Counting House Tape
// Session: A3, exercise 4 of 5 (6 hours for the evaluation)
// Theme: chapter 03 revision (buffered fread parsing at 1e7 scale; the
//   arithmetic is trivial on purpose)
// Tags: chapter-03, dynamic-programming
//
// MODEL: a running sum over a stream, tracking the minimum prefix value
//   including the empty prefix (balance 0 before any entry).
// MATH: B_0 = 0, B_i = B_{i-1} + e_i. Output B_n and min over i in
//   [0, n] of B_i. One pass, two accumulators.
// PROOF: nothing to prove about the recurrence; the content is the
//   input layer. At n = 1e7 signed tokens, cin with sync_with_stdio off
//   still pays per-token virtual-call overhead; a single fread into a
//   buffer and a hand parser (sign, digits, done) reads the tape at
//   memory speed. The chapter's measurements put roughly an order of
//   magnitude between the two; this exercise exists to make the reader
//   feel it under a clock.
// COMPLEXITY: O(n) time, O(1) memory beyond the 1 MB read buffer.
// TYPES: the balance drifts within +-(1e7 * 1e9) = +-1e16: long long
//   for both accumulators, and the parser accumulates digits into
//   long long as well (entries themselves fit in int, but parsing into
//   the wide type costs nothing and removes a cast).
// ALTERNATIVES: cin >> with sync off (works, slower, probably still
//   inside a 6-hour budget — the lesson is knowing the margin, not
//   superstition); mmap (chapter 03's other tool, fine here too);
//   istream iterators (elegant, slowest of all).
// EDGE CASES: n = 1; all debits (final = lowest); all credits (lowest
//   is 0, the empty prefix); the minimum reached at the very last entry;
//   entries split across line boundaries (the parser must not care about
//   newlines); the +-1e16 drift.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tape a3-04-counting-tape.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetape.exe a3-04-counting-tape.cpp

#include <algorithm>
#include <cstdio>
#include <print>
#include <vector>

namespace {

class FastReader {
public:
    FastReader() : buf_(1 << 20), pos_(0), len_(0) {}

    long long next() {
        int c = get();
        while (c != '-' && (c < '0' || c > '9')) c = get();
        bool negative = (c == '-');
        if (negative) c = get();
        long long value = 0;
        while (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
            c = get();
        }
        return negative ? -value : value;
    }

private:
    int get() {
        if (pos_ == len_) {
            len_ = std::fread(buf_.data(), 1, buf_.size(), stdin);
            pos_ = 0;
            if (len_ == 0) return -1;
        }
        return buf_[pos_++];
    }

    std::vector<unsigned char> buf_;
    std::size_t pos_;
    std::size_t len_;
};

}  // namespace

int main() {
    FastReader in;
    long long n = in.next();

    long long balance = 0;
    long long lowest = 0;  // the empty prefix counts
    for (long long i = 0; i < n; ++i) {
        balance += in.next();
        lowest = std::min(lowest, balance);
    }

    std::println("{} {}", balance, lowest);
    return 0;
}
