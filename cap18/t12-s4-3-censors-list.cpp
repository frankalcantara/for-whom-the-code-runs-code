// Exercise T12.7 - The Censor's List
// Session: T12.S4, exercise 3 of 3 (2 hours for all three; this is
//   the integrator)
// Theme: chapters 12 + 9 (Aho-Corasick: trie + KMP failure links
//   wired by BFS; counting by aggregation over the link tree)
// Tags: chapter-12, bfs, topological-sort, string-algorithms, trie, aho-corasick, kmp, brute-force
//
// MODEL: occurrence counts of k patterns in one text, overlaps
//   included - multi-pattern matching.
// MATH: build the trie of the patterns; the failure link of node v
//   points to the longest proper suffix of v's string that is also
//   a trie node (KMP's border, generalized). Feeding the text
//   through the goto/failure automaton visits, at step i, the node
//   of the longest pattern-prefix suffix ending at i. Counting:
//   increment visit[node] while scanning; afterwards, every
//   occurrence of a pattern P ending at i corresponds to a visited
//   node whose failure chain passes through P's terminal node, so
//   each pattern's count is the SUM of visit over its terminal
//   node's subtree in the failure-link tree - one upward
//   accumulation in reverse BFS order, no chain walking per
//   character.
// PROOF: (automaton) induction as in KMP: the failure chain of the
//   current node enumerates every suffix of the read text that is
//   a pattern prefix, longest first; the transition preserves the
//   "longest" invariant. (counting) P occurs ending at i iff P is
//   a suffix of the text at i iff P's node lies on the failure
//   chain of the node visited at step i; summing visit over the
//   failure-tree subtree of P's node counts exactly those steps.
//   Reverse BFS order is a valid bottom-up topological order
//   because links strictly decrease depth.
// COMPLEXITY: build O(sum |P| * 26) transitions precomputed
//   (goto-complete automaton: each node's 26 transitions filled
//   from its failure's); scan O(m); aggregate O(nodes). Memory:
//   nodes <= sum |P| + 1 <= 1e6 + 1, times 26 int32 transitions
//   ~ 108 MB worst case - heavy but within 256 MB; the lazy
//   (non-complete) variant with on-the-fly failure walks trades
//   that memory for amortized scan time.
// TYPES: counts can reach m = 1e6 per pattern (int fits, long long
//   used for the totals out of caution); node indices int32.
//   Duplicate patterns on the list share a terminal node - each
//   query line keeps its own node reference, so duplicates report
//   equal counts without special casing.
// ALTERNATIVES: KMP per pattern is O(k * m) = 2e11, dead; a
//   suffix array of the text answers each pattern by binary
//   search in O(|P| log m) - a genuine rival at these limits,
//   with the same overlap-inclusive counts; Aho-Corasick wins on
//   being one pass over the text and is the tool this unit
//   trains.
// EDGE CASES: pattern absent (0); pattern equal to the whole text;
//   overlapping self-similar patterns (aba in ababab -> 2); one
//   pattern a suffix of another (both must count when the longer
//   one matches); duplicate list entries; single-letter patterns.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o censor t12-s4-3-censors-list.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecensor.exe t12-s4-3-censors-list.cpp

#include <array>
#include <cstdint>
#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace {

struct AhoCorasick {
    std::vector<std::array<std::int32_t, 26>> go{
        std::array<std::int32_t, 26>{}};
    std::vector<std::int32_t> fail{0};
    std::vector<long long> visit{0};

    std::int32_t add_word(const std::string& word) {
        std::int32_t v = 0;
        for (const char ch : word) {
            const auto c = static_cast<std::size_t>(ch - 'a');
            std::int32_t next = go[static_cast<std::size_t>(v)][c];
            if (next == 0) {
                // No reference into go survives the push_back below:
                // reallocation would dangle it.
                next = static_cast<std::int32_t>(go.size());
                go.push_back({});
                fail.push_back(0);
                visit.push_back(0);
                go[static_cast<std::size_t>(v)][c] = next;
            }
            v = next;
        }
        return v;
    }

    // Completes transitions and failure links; returns BFS order.
    std::vector<std::int32_t> build() {
        std::vector<std::int32_t> order;
        order.reserve(go.size());
        for (std::size_t c = 0; c < 26; ++c) {
            if (go[0][c] != 0) order.push_back(go[0][c]);
        }
        for (std::size_t head = 0; head < order.size(); ++head) {
            const auto v = static_cast<std::size_t>(order[head]);
            for (std::size_t c = 0; c < 26; ++c) {
                const std::int32_t u = go[v][c];
                const std::int32_t via_fail =
                    go[static_cast<std::size_t>(fail[v])][c];
                if (u == 0) {
                    go[v][c] = via_fail;  // complete the automaton
                } else {
                    fail[static_cast<std::size_t>(u)] = via_fail;
                    order.push_back(u);
                }
            }
        }
        return order;
    }
};

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;
    std::size_t k = 0;
    std::cin >> k;

    AhoCorasick ac;
    std::vector<std::int32_t> terminal(k);
    {
        std::string word;
        for (std::size_t i = 0; i < k; ++i) {
            std::cin >> word;
            terminal[i] = ac.add_word(word);
        }
    }
    const std::vector<std::int32_t> order = ac.build();

    std::int32_t v = 0;
    for (const char ch : text) {
        const auto c = static_cast<std::size_t>(ch - 'a');
        v = ac.go[static_cast<std::size_t>(v)][c];
        ++ac.visit[static_cast<std::size_t>(v)];
    }
    // Accumulate visits up the failure tree, children before parents.
    for (std::size_t i = order.size(); i > 0; --i) {
        const auto node = static_cast<std::size_t>(order[i - 1]);
        ac.visit[static_cast<std::size_t>(ac.fail[node])] += ac.visit[node];
    }

    std::string out;
    for (std::size_t i = 0; i < k; ++i) {
        out += std::to_string(ac.visit[static_cast<std::size_t>(terminal[i])]);
        out += '\n';
    }
    std::print("{}", out);
    return 0;
}
