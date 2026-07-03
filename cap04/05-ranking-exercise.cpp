// cap04/05-ranking-exercise.cpp
// Solved exercise: Contest Ranking
//
// Given n contestants with {id, score, name}:
//   (a) Find the k-th highest score using binary search on answers.
//   (b) List all contestants with that score in lexicographic name order.
//   (c) Count how many contestants have score above threshold T.
//
// Input:  line 1 = n k T
//         lines 2..n+1 = id score name
// Output: k-th score, name list, count above T
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o ranking 05-ranking-exercise.cpp
// Test:    printf "5 2 80\n1 95 Alice\n2 80 Bruno\n3 80 Carlos\n4 72 Diana\n5 88 Eve\n" | ./ranking

#include <algorithm>
#include <climits>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <vector>

struct Contestant {
    int id, score;
    std::string name;
};

// ============================================================
// (a) k-th highest score by binary search on answers.
// Predicate: how many contestants have score >= mid? Is that count >= k?
// Time: O(n log(max_score))
// ============================================================
int kth_score(const std::vector<Contestant>& cs, int k) {
    int lo = 0, hi = 100'000;  // [0, max_score]

    // Find the largest score S such that |{score >= S}| >= k.
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        int cnt = static_cast<int>(
            std::ranges::count_if(cs, [mid](const Contestant& c) {
                return c.score >= mid;
            })
        );
        if (cnt >= k) lo = mid;    // at least k contestants have score >= mid
        else          hi = mid - 1;
    }
    return lo;
}

// ============================================================
// (b) Contestants with score == target, sorted by name.
// Layer 2: filter + sort.
// Time: O(n + d log d) onde d = |{score == target}|
// ============================================================
std::vector<std::string> names_with_score(
    const std::vector<Contestant>& cs, int target)
{
    std::vector<std::string> result;
    result.reserve(cs.size());

    for (const auto& c : cs)
        if (c.score == target)
            result.push_back(c.name);

    std::ranges::sort(result);  // O(d log d), d <= n
    return result;
}

// ============================================================
// (c) Count scores above T using upper_bound.
// Requires a sorted score array, O(log n) after O(n log n) sorting.
// ============================================================
int count_above(const std::vector<int>& sorted_scores, int threshold) {
    // upper_bound returns the first score > threshold.
    auto it = std::ranges::upper_bound(sorted_scores, threshold);
    return static_cast<int>(sorted_scores.end() - it);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k, T;
    std::cin >> n >> k >> T;

    std::vector<Contestant> cs(n);
    for (auto& c : cs) std::cin >> c.id >> c.score >> c.name;

    // (a)
    int ks = kth_score(cs, k);
    std::print("k-th score: {}\n", ks);

    // (b)
    auto names = names_with_score(cs, ks);
    std::print("contestants with score {}: ", ks);
    for (const auto& name : names) std::print("{} ", name);
    std::print("\n");

    // (c): build a sorted score vector for upper_bound.
    std::vector<int> scores;
    scores.reserve(n);
    for (const auto& c : cs) scores.push_back(c.score);
    std::ranges::sort(scores);
    std::print("above {}: {}\n", T, count_above(scores, T));

    return 0;
}
