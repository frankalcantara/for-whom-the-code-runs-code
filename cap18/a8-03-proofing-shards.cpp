// Exercise A8.3 - The Proofing Shards
// Session: A8, exercise 3 of 10
// Theme: chapters 12 and 16 (string scanning split across futures)
// Tags: chapter-12, chapter-16
//
// MODEL: count pattern occurrences by assigning ranges of starting
//   positions to asynchronous tasks.
// MATH: a start position s is a match iff text[s+i]==pattern[i] for
//   all i. The valid start positions form [0, n-m]. This range is
//   partitioned among tasks.
// PROOF: every occurrence has exactly one start position. The task
//   ranges are disjoint and cover all starts, so summing their
//   counts counts every occurrence exactly once, including overlaps.
// COMPLEXITY: O((n-m+1)m) character checks, with m<=100, and O(p)
//   futures.
// TYPES: counts fit int64_t.
// ALTERNATIVES: KMP is asymptotically better; here the short phrase
//   bound makes direct comparisons acceptable while training task
//   splitting.
// EDGE CASES: overlapping matches; pattern length 1; no matches.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o shards a8-03-proofing-shards.cpp

#include <algorithm>
#include <cstdint>
#include <future>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int workers = 0;
    std::string pattern;
    std::string text;
    std::cin >> workers >> pattern >> text;

    const int starts = static_cast<int>(text.size() - pattern.size() + 1);
    workers = std::min(workers, starts);
    std::vector<std::future<std::int64_t>> futures;
    for (int id = 0; id < workers; ++id) {
        const int lo = id * starts / workers;
        const int hi = (id + 1) * starts / workers;
        futures.push_back(std::async(std::launch::async, [&, lo, hi] {
            std::int64_t local = 0;
            for (int s = lo; s < hi; ++s) {
                bool ok = true;
                for (std::size_t i = 0; i < pattern.size(); ++i) {
                    if (text[static_cast<std::size_t>(s) + i] != pattern[i]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) ++local;
            }
            return local;
        }));
    }

    std::int64_t answer = 0;
    for (auto& future : futures) answer += future.get();
    std::print("{}\n", answer);
    return 0;
}
