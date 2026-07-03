// Exercise A4.1 - The Parlor Word Chain
// Session: A4, exercise 1 of 6 (6 hours for the evaluation)
// Theme: chapter 07 (BFS over an implicit graph), with chapter 06's
//   hash set as the dictionary
// Tags: chapter-07, chapter-06, bfs, brute-force
//
// MODEL: an implicit graph whose vertices are the dictionary words and
//   whose edges join words differing in exactly one position. The
//   answer is the BFS distance between the two given words. The graph
//   is never built: neighbors are GENERATED, by mutating one letter at
//   a time and asking the hash set whether the result is a word.
// MATH: a word of length L has 25L candidate mutations; with n words,
//   BFS touches each word once and examines at most 25L candidates
//   from it, so the work is O(n L^2 * alpha) string operations (the
//   extra L is hashing the mutated copy; alpha the hash-set constant).
//   At n = 5000, L = 8: about 1e6 lookups on short strings.
// PROOF: BFS distance correctness (induction on layers, as in the
//   chapter); the mutation enumeration generates exactly the edge set
//   - every neighbor differs in exactly one position, and every
//   one-position variant is tried - so the BFS runs on the true graph.
//   Generating neighbors beats precomputing all O(n^2) pairs whenever
//   25L << n, which at these limits is 200 vs 5000.
// COMPLEXITY: O(n L^2) expected time, O(n) memory for the set and
//   distances. The pairwise-comparison construction is O(n^2 L) = 2e8
//   character work: borderline, and pointless when the alphabet trick
//   exists.
// TYPES: distances at most n, int; the words live in std::string;
//   an unordered_map<string, int> keeps distances and doubles as the
//   visited set. No arithmetic to overflow anywhere - the danger in
//   string problems is allocation, not width (mutate one buffer in
//   place, restore the letter after each probe).
// ALTERNATIVES: bidirectional BFS halves the explored layers (worth it
//   when L grows); the wildcard-bucket construction (file each word
//   under its L patterns like c*ld) builds explicit adjacency in
//   O(n L) buckets and is the standard heavy-duty answer; A* with
//   Hamming-distance heuristic is admissible and rarely worth the
//   code in a timed room.
// EDGE CASES: start equals goal (0); goal unreachable (disconnected
//   component - the example's "wood" is bait for this in variants);
//   length-1 words (any word reaches any other in one step if both
//   present); duplicated dictionary entries (set semantics absorb
//   them).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o chain a4-01-parlor-word-chain.cpp

#include <iostream>
#include <print>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::unordered_set<std::string> dictionary;
    dictionary.reserve(static_cast<std::size_t>(n) * 2);
    for (int i = 0; i < n; ++i) {
        std::string w;
        std::cin >> w;
        dictionary.insert(std::move(w));
    }

    std::string start;
    std::string goal;
    std::cin >> start >> goal;

    std::unordered_map<std::string, int> dist;
    dist.reserve(static_cast<std::size_t>(n) * 2);
    std::queue<std::string> frontier;
    dist[start] = 0;
    frontier.push(start);

    while (!frontier.empty() && dist.find(goal) == dist.end()) {
        std::string word = std::move(frontier.front());
        frontier.pop();
        const int d = dist[word];
        for (std::size_t pos = 0; pos < word.size(); ++pos) {
            const char original = word[pos];
            for (char letter = 'a'; letter <= 'z'; ++letter) {
                if (letter == original) continue;
                word[pos] = letter;
                if (dictionary.contains(word) && dist.find(word) == dist.end()) {
                    dist[word] = d + 1;
                    frontier.push(word);
                }
            }
            word[pos] = original;  // restore before the next position
        }
    }

    const auto it = dist.find(goal);
    if (it == dist.end()) {
        std::print("impossible\n");
    } else {
        std::print("{}\n", it->second);
    }
    return 0;
}
