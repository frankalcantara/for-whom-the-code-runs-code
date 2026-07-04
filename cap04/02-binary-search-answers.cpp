// cap04/02-binary-search-answers.cpp
// Binary search on answers: standard monotone-predicate pattern.
// Classic problem: k workers must process n tasks with times t[i].
// Each worker receives consecutive tasks. Minimize the maximum worker time.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o bsearch_ans 02-binary-search-answers.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febsearch_ans.exe 02-binary-search-answers.cpp
// Test:    echo "6 3 1 2 3 4 5 6" | ./bsearch_ans   expected: 7

#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <print>
#include <vector>

// Checks whether tasks can be distributed among k workers
// with each worker receiving at most `limit` total time.
// Greedy: assign tasks to the current worker until the limit would be exceeded.
// Time: O(n)
bool feasible(const std::vector<int>& t, int k, long long limit) {
    int workers = 1;
    long long current = 0;
    for (int ti : t) {
        if (ti > limit) return false;   // one task alone exceeds the limit
        if (current + ti > limit) {
            ++workers;                  // need a new worker
            current = 0;
            if (workers > k) return false;
        }
        current += ti;
    }
    return true;
}

// Binary search on answers: finds the smallest `limit` such that feasible(limit) is true.
// The predicate is monotone: if limit=L works, then L+1 also works.
// Search interval: [max(t), sum(t)], smallest when k==n, largest when k==1.
// Time: O(n log(sum(t)))
long long min_makespan(const std::vector<int>& t, int k) {
    long long lo = *std::ranges::max_element(t);        // smallest possible limit
    long long hi = std::accumulate(t.begin(), t.end(), 0LL); // largest possible limit

    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(t, k, mid)) hi = mid;   // mid is feasible; try smaller
        else                     lo = mid + 1; // mid is not feasible; increase
    }
    return lo;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    std::vector<int> t(n);
    for (int& x : t) std::cin >> x;

    std::print("{}\n", min_makespan(t, k));
    return 0;
}
