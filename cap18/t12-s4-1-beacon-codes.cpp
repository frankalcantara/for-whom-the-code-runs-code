// Exercise T12.5 - The Beacon Codes
// Session: T12.S4, exercise 1 of 3 (2 hours for all three)
// Theme: chapter 12 (XOR trie / binary trie greedy)
// Tags: chapter-12, greedy, string-algorithms, trie
//
// MODEL: maximize a_i XOR a_j over pairs - maximum XOR pair.
// MATH: insert each code into a binary trie over its 30 bits, most
//   significant first. For a query value x, walk the trie taking
//   at each level the child with the OPPOSITE bit of x when it
//   exists (it contributes 2^level to the XOR), the same bit
//   otherwise. The walk maximizes x XOR y over all stored y.
//   Stream the array: query each a_i against the trie of
//   a_1..a_{i-1}, then insert a_i - every pair is examined exactly
//   once as (later, earlier).
// PROOF: greedy by bit dominance - 2^L exceeds the sum of all
//   lower powers, so any answer that wins bit L beats every answer
//   that loses it, regardless of the tail; at each level the
//   opposite-bit branch wins bit L if any stored value lives
//   there. Induction down the levels gives optimality of the walk;
//   the streaming order covers all unordered pairs once.
// COMPLEXITY: O(n * 30) time and at most n*30 + 1 trie nodes -
//   ~6e6 nodes worst case at 8 bytes each (two int32 children):
//   ~48 MB. Fits; a tighter pool would reserve up front.
// TYPES: codes < 2^30 fit int32; XOR of two such values also
//   < 2^30. Node indices int32 (< 6e6+2). No widening needed
//   anywhere - the exercise's rare mercy.
// ALTERNATIVES: sort-based bit-group partitioning achieves
//   O(n log n log A) with less memory and more thought; trying all
//   pairs is O(n^2) = 4e10, dead. The trie is also the form that
//   answers ONLINE queries (insert/query interleaved), which the
//   relay committee will inevitably request next winter.
// EDGE CASES: n = 2 (single pair); all codes equal (answer 0);
//   a code of 0 against the maximum (answer = maximum); duplicate
//   codes (trie holds both harmlessly); codes touching 2^30 - 1.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o beacons t12-s4-1-beacon-codes.cpp

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

constexpr int kBits = 30;

struct XorTrie {
    // children[v][b]: index of child for bit b; 0 = absent.
    std::vector<std::array<std::int32_t, 2>> children{
        std::array<std::int32_t, 2>{}};

    void insert(std::int32_t value) {
        std::size_t v = 0;
        for (int level = kBits - 1; level >= 0; --level) {
            const std::size_t b = static_cast<std::size_t>(value >> level) & 1;
            if (children[v][b] == 0) {
                children[v][b] = static_cast<std::int32_t>(children.size());
                children.push_back({});
            }
            v = static_cast<std::size_t>(children[v][b]);
        }
    }

    std::int32_t best_xor(std::int32_t value) const {
        std::size_t v = 0;
        std::int32_t result = 0;
        for (int level = kBits - 1; level >= 0; --level) {
            const std::size_t b = static_cast<std::size_t>(value >> level) & 1;
            if (children[v][1 - b] != 0) {
                result |= std::int32_t{1} << level;
                v = static_cast<std::size_t>(children[v][1 - b]);
            } else {
                v = static_cast<std::size_t>(children[v][b]);
            }
        }
        return result;
    }
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;

    XorTrie trie;
    std::int32_t answer = 0;
    for (std::size_t i = 0; i < n; ++i) {
        std::int32_t code = 0;
        std::cin >> code;
        if (i > 0) answer = std::max(answer, trie.best_xor(code));
        trie.insert(code);
    }
    std::print("{}\n", answer);
    return 0;
}
