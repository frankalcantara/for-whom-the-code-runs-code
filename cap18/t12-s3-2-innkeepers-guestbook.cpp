// Exercise T12.4 - The Innkeeper's Guestbook
// Session: T12.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 12 (trie with subtree counters)
// Tags: chapter-12, string-algorithms, trie
//
// MODEL: multiset of strings; queries ask how many members have a
//   given prefix.
// MATH: a trie where every node stores pass = number of inserted
//   names whose path crosses the node. Inserting a name increments
//   pass along its path; a query walks its prefix and reports the
//   pass counter of the node it lands on (0 if it falls off).
// PROOF: a name has prefix x iff its path crosses the node that
//   spells x - paths in a trie are exactly the prefixes; the
//   counter is maintained additively per insertion, so it equals
//   the number of crossing names at all times. Repetitions are
//   counted per insertion, as the guestbook requires.
// COMPLEXITY: O(L) time, L <= 1e6 the total input length - each
//   character of each name or query advances one trie edge in
//   O(1). Memory: one node per distinct inserted prefix, at most
//   total inserted length + 1 nodes; each node carries a 26-slot
//   child table of int32 plus a counter, ~108 bytes. Worst case
//   ~1e6 nodes ~ 108 MB: inside a 256 MB limit, and the pool grows
//   lazily so benign inputs stay small.
// TYPES: node indices int32 (node count <= 1e6 + 1, and 0 doubles
//   as "absent" since the root is never a child); pass counters
//   int32 (at most n = 2e5 insertions cross any node).
// ALTERNATIVES: sort the names; a prefix query is an
//   equal_range over [x, x + CHAR_MAX) via lower_bound on x and on
//   x with last char bumped - O(L log n) total, less memory, the
//   chapter 4 answer. The trie is the chapter 12 answer and the
//   one that extends to per-node aggregates no order statistic
//   reaches. Hash map keyed by every prefix: O(L) memory blowup
//   times average name length - no.
// EDGE CASES: query longer than any name (falls off: 0); query
//   equal to a full name (counts it); repeated names (each stay
//   counts); single-letter names and queries; query at a node with
//   no inserted name ending there but many passing through.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o guestbook t12-s3-2-innkeepers-guestbook.cpp

#include <array>
#include <cstdint>
#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace {

struct Trie {
    // children[v][c] = index of child, 0 = absent (node 0 is root,
    // never a child). pass[v] = names whose path crosses v.
    std::vector<std::array<std::int32_t, 26>> children{
        std::array<std::int32_t, 26>{}};  // root only, all absent
    std::vector<std::int32_t> pass{0};

    void insert(const std::string& name) {
        std::int32_t v = 0;
        for (const char ch : name) {
            const auto c = static_cast<std::size_t>(ch - 'a');
            if (children[v][c] == 0) {
                children[v][c] = static_cast<std::int32_t>(children.size());
                children.push_back({});
                pass.push_back(0);
            }
            v = children[v][c];
            ++pass[static_cast<std::size_t>(v)];
        }
    }

    std::int32_t count(const std::string& prefix) const {
        std::int32_t v = 0;
        for (const char ch : prefix) {
            const auto c = static_cast<std::size_t>(ch - 'a');
            v = children[static_cast<std::size_t>(v)][c];
            if (v == 0) return 0;
        }
        return pass[static_cast<std::size_t>(v)];
    }
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    Trie trie;
    std::string line;
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> line;
        trie.insert(line);
    }
    std::string out;
    for (std::size_t i = 0; i < q; ++i) {
        std::cin >> line;
        out += std::to_string(trie.count(line));
        out += '\n';
    }
    std::print("{}", out);
    return 0;
}
