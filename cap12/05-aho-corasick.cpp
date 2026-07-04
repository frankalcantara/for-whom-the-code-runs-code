// cap12/05-aho-corasick.cpp
// Aho-Corasick multi-pattern string matching.
//
// Builds a trie from a set of patterns, then adds failure links
// (analogous to the KMP failure function) via BFS.
// After construction, searches a text in O(n + total_matches).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o aho-corasick 05-aho-corasick.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feaho-corasick.exe 05-aho-corasick.cpp

#include <array>
#include <print>
#include <queue>
#include <string>
#include <utility>
#include <vector>

// ===== Aho-Corasick Automaton =====
//
// Each trie node stores:
//   ch[c]  -- transition on character c (or -1 if absent before build)
//   fail   -- failure link: longest proper suffix of this node's string
//             that is a prefix of any pattern
//   pat_id -- index of pattern ending here (-1 if none)
//   dict   -- dictionary link: nearest ancestor via fail links that
//             is also an accepting state (for counting multiple patterns)
//
// After build(), ch[c] is filled for all characters and all nodes,
// so the automaton never needs to follow fail links during search.
// That transformation is the key efficiency: every character in the
// text advances the state exactly once.

struct AhoCorasick {
    static constexpr int ALPHA = 26;

    struct Node {
        std::array<int, ALPHA> ch;  // transitions
        int fail;              // failure link
        int pat_id;            // which pattern ends here (-1 = none)
        int dict;              // nearest accepting ancestor via fail chain
        Node() : fail(0), pat_id(-1), dict(-1) { ch.fill(-1); }
    };

    std::vector<Node> t;

    AhoCorasick() {
        t.emplace_back();  // node 0 is the root
    }

    // Insert a pattern and tag its terminal node with id.
    void insert(const std::string& s, int id) {
        int cur = 0;
        for (char c : s) {
            int ci = c - 'a';
            if (t[cur].ch[ci] == -1) {
                t[cur].ch[ci] = (int)t.size();
                t.emplace_back();
            }
            cur = t[cur].ch[ci];
        }
        t[cur].pat_id = id;
    }

    // Build failure links and complete the automaton via BFS.
    //
    // For root's children: fail points to root.
    // For any other node u reached by character c from parent p:
    //   fail[u] = ch[fail[p]][c]
    // where ch is the completed automaton of fail[p].
    //
    // After this pass, ch[u][c] is never -1: missing transitions
    // are redirected to follow the failure link automaton.
    // This makes the automaton deterministic with no fail-following
    // needed at search time.
    void build() {
        std::queue<int> q;

        // Initialize root's direct children.
        for (int c = 0; c < ALPHA; ++c) {
            if (t[0].ch[c] == -1) {
                t[0].ch[c] = 0;  // redirect missing root transitions to root
            } else {
                t[t[0].ch[c]].fail = 0;
                q.push(t[0].ch[c]);
            }
        }

        while (!q.empty()) {
            int u = q.front(); q.pop();

            // Dictionary link: nearest fail-ancestor that is accepting.
            int f = t[u].fail;
            if (t[f].pat_id != -1)
                t[u].dict = f;
            else
                t[u].dict = t[f].dict;

            for (int c = 0; c < ALPHA; ++c) {
                if (t[u].ch[c] == -1) {
                    // No trie edge: follow fail link automaton.
                    t[u].ch[c] = t[f].ch[c];
                } else {
                    // Trie edge exists: set its failure link.
                    t[t[u].ch[c]].fail = t[f].ch[c];
                    q.push(t[u].ch[c]);
                }
            }
        }
    }

    // Search text for all pattern occurrences.
    // Returns a list of (position, pattern_id) pairs (position = end index).
    // Counts matches from both pat_id and the dict link chain.
    std::vector<std::pair<int, int>> search(const std::string& txt) {
        std::vector<std::pair<int, int>> results;
        int cur = 0;
        for (int i = 0; i < (int)txt.size(); ++i) {
            // Advance automaton state. After build(), this is always valid.
            cur = t[cur].ch[txt[i] - 'a'];

            // Report if a pattern ends at this state.
            if (t[cur].pat_id != -1)
                results.push_back({i, t[cur].pat_id});

            // Follow dict links to report all patterns ending here.
            for (int d = t[cur].dict; d != -1; d = t[d].dict)
                results.push_back({i, t[d].pat_id});
        }
        return results;
    }
};

// ===== Demo =====

int main() {
    // Classic Aho-Corasick example from Aho & Corasick (1975).
    // Patterns: "he", "she", "his", "hers"
    // Text: "ushers"
    // Expected matches: "she" at position 4, "he" at position 4, "hers" at position 5.

    std::vector<std::string> patterns = {"he", "she", "his", "hers"};
    AhoCorasick ac;
    for (int i = 0; i < (int)patterns.size(); ++i)
        ac.insert(patterns[i], i);
    ac.build();

    std::string text = "ushers";
    auto results = ac.search(text);

    std::print("Text: {}\n", text);
    for (auto [pos, pid] : results)
        std::print("  pattern \"{}\" ends at index {}\n", patterns[pid], pos);
    // he   ends at 4
    // she  ends at 4
    // hers ends at 5

    return 0;
}
