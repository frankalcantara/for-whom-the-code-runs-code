// Exercise T03.3 - The Antique Adding Machine
// Session: T03.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 03 (decimal parsing semantics: signs, padding, accumulation)
// Tags: chapter-03, dynamic-programming
//
// MODEL: sum n integers written with an optional sign and arbitrary
//   leading zeros.
// MATH: positional notation makes leading zeros inert: the parse
//   recurrence value = 10 * value + digit starts at 0, and feeding it
//   zeros first leaves value at 0. So `+0042` and `42` produce the same
//   number by construction, not by a special case. The sign is a separate
//   prefix applied once, after the magnitude.
// PROOF: induction over the digits d_0..d_{k-1}: after consuming d_i, the
//   accumulator holds the value of the prefix read so far in base ten.
//   The final multiplication by the sign yields the token's value, and
//   addition over tokens is associative.
// COMPLEXITY: O(total characters) time, O(1) memory per token.
// TYPES: tokens reach 18 digits, up to ~1e18: parse into long long. The
//   guaranteed-fitting total also lives in long long. A 19th digit would
//   demand the unsigned-magnitude care of Chapter 3's edge-of-range
//   parser; the constraint here deliberately stops one digit short of
//   that cliff, and the header says so instead of pretending the cliff
//   does not exist.
// ALTERNATIVES: std::stoll parses sign and zeros identically but charges
//   one std::string per token; stream extraction into long long also
//   works and is the honest short answer; the manual parse is shown
//   because this unit is about owning the semantics, not outsourcing them.
// EDGE CASES: `0000` and `+0` and `-0` (all zero); a bare unsigned token;
//   the maximum 18-digit magnitude; a sum that cancels to zero.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o adding t03-s3-1-adding-machine.cpp

#include <iostream>
#include <print>
#include <string>

long long parse_signed(const std::string& token) {
    std::size_t i = 0;
    long long sign = 1;
    if (token[0] == '+') { i = 1; }
    else if (token[0] == '-') { sign = -1; i = 1; }

    long long value = 0;
    for (; i < token.size(); ++i)
        value = value * 10 + (token[i] - '0');
    return sign * value;
}

int main() {
    int n = 0;
    std::cin >> n;

    long long total = 0;
    for (int i = 0; i < n; ++i) {
        std::string token;
        std::cin >> token;
        total += parse_signed(token);
    }

    std::print("{}\n", total);
    return 0;
}
