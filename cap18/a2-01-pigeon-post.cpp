// Exercise A2.1 - The Pigeon Post
// Session: A2, exercise 1 of 4 (5 hours for the evaluation)
// Theme: chapter 03 (streaming text records at 1e6 scale; owning the key,
//   not the file)
// Tags: chapter-03
//
// MODEL: filter-and-aggregate over a stream of (name, weight) records:
//   count and sum the records whose name equals the audited town.
// MATH: each record contributes independently, so one pass with two
//   accumulators suffices; nothing about earlier records changes how a
//   later record is judged. The season's file never needs to exist in
//   memory: only the audited name, the current record, and two numbers.
// PROOF: fold invariant. After i records, `tubes` and `grams` equal the
//   count and weight-sum of the matching records among the first i.
//   Equality of names is exact string equality; induction on i.
// COMPLEXITY: O(total characters) time, O(1) memory beyond the current
//   record. With n = 1e6 lines, unsynced streams carry the volume; the
//   fread parser would be the next layer if names grew or limits dropped.
// TYPES: total weight reaches 1e6 * 1e9 = 1e15: long long. The token is
//   read into one reused std::string; comparing it with the audited name
//   compares characters, not pointers, and allocates nothing new per line
//   beyond the buffer the string already owns.
// ALTERNATIVES: a hash map from every town to its totals computes all
//   audits at once and is the right tool when the auditor asks about many
//   towns; for one town it stores a season of strangers to answer one
//   question. Reading lines and splitting by hand adds parsing surface
//   with no gain over stream extraction here.
// EDGE CASES: no tube to the audited town (0 0); every tube to the
//   audited town; the audited name sharing a prefix with another town
//   (ravenna vs ravensburg must not match); n = 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o pigeon a2-01-pigeon-post.cpp

#include <iostream>
#include <print>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string audited;
    long long n = 0;
    std::cin >> audited >> n;

    long long tubes = 0;
    long long grams = 0;
    std::string town;
    for (long long i = 0; i < n; ++i) {
        long long weight = 0;
        std::cin >> town >> weight;
        if (town == audited) {
            ++tubes;
            grams += weight;
        }
    }

    std::print("{} {}\n", tubes, grams);
    return 0;
}
