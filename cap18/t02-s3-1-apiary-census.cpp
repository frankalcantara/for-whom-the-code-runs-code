// Exercise T02.3 - The Apiary Census
// Session: T02.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 02 (frequency counting over sparse keys, explicit tie rule)
// Tags: chapter-02
//
// MODEL: count occurrences of each tag; select the tag maximizing
//   (count, -tag) in lexicographic order, i.e., highest count, then
//   smallest tag.
// MATH: tags reach 1e9, so a counting array over the domain is 4 GB of
//   apology; the key space is sparse (at most n distinct tags), which is
//   the signature of a hash table. One pass builds the frequencies; one
//   pass over the distinct keys selects the winner.
// PROOF: fold invariant. After scanning i census entries, the map holds the
//   exact multiset of the first i tags. The selection scan maintains the
//   best (count, tag) pair seen so far, replacing it only on strictly
//   higher count or equal count with smaller tag; induction over distinct
//   keys gives the contract's winner regardless of hash iteration order.
// COMPLEXITY: expected O(n) for counting, O(d) for selection over d
//   distinct tags; O(d) memory. reserve(n) avoids rehashing mid-input.
// TYPES: counts fit in int (<= n = 2e5); tags fit in int (<= 1e9 < 2^31-1).
//   long long would also work and cost nothing; int is enough and says so.
// ALTERNATIVES: sort-and-scan groups equal tags in O(n log n) and gives
//   the smallest-tag tie rule for free; std::map costs O(log) per insert
//   for an ordering the problem only needs at the very end. The hash table
//   matches the question: membership and counting, no order.
// EDGE CASES: n = 1; all tags equal; all tags distinct (every count 1, the
//   smallest tag wins); the winner appearing last in the input.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o apiary t02-s3-1-apiary-census.cpp

#include <iostream>
#include <print>
#include <unordered_map>

int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<int, int> freq;
    freq.reserve(n);

    for (int i = 0; i < n; ++i) {
        int tag = 0;
        std::cin >> tag;
        ++freq[tag];
    }

    int best_tag = 0;
    int best_count = 0;
    for (const auto& [tag, count] : freq) {
        if (count > best_count ||
            (count == best_count && tag < best_tag)) {
            best_tag = tag;
            best_count = count;
        }
    }

    std::print("{} {}\n", best_tag, best_count);
    return 0;
}
