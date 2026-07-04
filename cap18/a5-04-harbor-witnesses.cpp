// Exercise A5.4 - The Harbor Witnesses
// Session: A5, exercise 4 of 7 (6 hours for the evaluation)
// Theme: chapter 08 revision (longest common subsequence; two-row
//   rolling table)
// Tags: chapter-08, greedy, dynamic-programming, lcs
//
// MODEL: the longest common subsequence of two sequences A[1..n] and
//   B[1..m]. The witnesses' "part of the night both can swear to" is
//   exactly a common subsequence: order preserved, gaps allowed,
//   contiguity not required.
// MATH: L[i][j] = LCS length of the prefixes A[1..i], B[1..j], with
//   L[i][0] = L[0][j] = 0 and the recurrence
//     L[i][j] = L[i-1][j-1] + 1                  if A[i] == B[j],
//     L[i][j] = max(L[i-1][j], L[i][j-1])        otherwise.
//   Row i depends only on row i-1, so two rows of m+1 ints suffice.
// PROOF: exchange argument on the last element. If A[i] == B[j],
//   some optimal common subsequence of the two prefixes ends by
//   matching A[i] with B[j]: an optimum that uses neither can be
//   extended (contradiction with optimality), and one that uses only
//   one of them can be rerouted to the pair without loss. If
//   A[i] != B[j], any common subsequence must omit A[i] or B[j], so
//   the optimum is the better of the two shorter prefixes. Induction
//   over (i, j) closes the argument.
// COMPLEXITY: O(nm) time = 9e6 table cells at the limits, O(m)
//   memory with the rolling pair of rows. Reconstruction would need
//   the full table (or Hirschberg's divide and conquer), but the
//   harbormaster asked for the length: do not volunteer for work the
//   statement did not assign.
// TYPES: lengths are at most min(n, m) = 3000, so int everywhere;
//   hull numbers reach 1e9, which fits int, and they are only ever
//   compared for equality - never summed, so no width anxiety
//   (chapter 01's checklist, applied and dismissed in one line).
// ALTERNATIVES: LIS in n log n (chapter 8) does not apply directly -
//   the reduction LCS -> LIS is only efficient when one sequence has
//   distinct elements, which the statement does not promise; hashing
//   positions (chapter 6) buys nothing because order, not identity,
//   is the constraint; edit distance computes a different currency
//   (substitutions are allowed there, only matches count here).
// EDGE CASES: no hull number in common (answer 0); identical lists
//   (answer n); one list of length 1; repeated hull numbers inside a
//   single list (the recurrence never assumed distinctness); answer
//   achieved by two different subsequences, as in the example.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o witnesses a5-04-harbor-witnesses.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fewitnesses.exe a5-04-harbor-witnesses.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<int> first_list(static_cast<std::size_t>(n));
    for (auto& hull : first_list) {
        std::cin >> hull;
    }

    int m = 0;
    std::cin >> m;
    std::vector<int> second_list(static_cast<std::size_t>(m));
    for (auto& hull : second_list) {
        std::cin >> hull;
    }

    std::vector<int> previous(static_cast<std::size_t>(m) + 1, 0);
    std::vector<int> current(static_cast<std::size_t>(m) + 1, 0);

    for (int i = 1; i <= n; ++i) {
        const int a = first_list[static_cast<std::size_t>(i - 1)];
        for (int j = 1; j <= m; ++j) {
            const auto js = static_cast<std::size_t>(j);
            if (a == second_list[js - 1]) {
                current[js] = previous[js - 1] + 1;
            } else {
                current[js] = std::max(previous[js], current[js - 1]);
            }
        }
        std::swap(previous, current);
    }

    std::println("{}", previous[static_cast<std::size_t>(m)]);
    return 0;
}
