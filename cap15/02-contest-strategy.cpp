// cap15/02-contest-strategy.cpp
// Solved strategy patterns: interval piercing, circular subarray, and bounded BFS.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o contest_strategy 02-contest-strategy.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecontest_strategy.exe 02-contest-strategy.cpp

#include <algorithm>
#include <cassert>
#include <climits>
#include <print>
#include <utility>
#include <vector>

using i64 = long long;

int min_points_to_pierce(std::vector<std::pair<int, int>> intervals) {
    std::sort(intervals.begin(), intervals.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    int count = 0;
    int last = INT_MIN;
    for (auto [l, r] : intervals) {
        if (l > last) {
            ++count;
            last = r;
        }
    }
    return count;
}

i64 max_circular_subarray(const std::vector<int>& a) {
    if (a.empty()) return 0;

    i64 max_lin = a[0], cur = a[0];
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        cur = std::max<i64>(a[i], cur + a[i]);
        max_lin = std::max(max_lin, cur);
    }

    i64 total = 0, min_sub = a[0], cur_min = a[0];
    for (int x : a) total += x;
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        cur_min = std::min<i64>(a[i], cur_min + a[i]);
        min_sub = std::min(min_sub, cur_min);
    }

    if (total == min_sub) return max_lin;
    return std::max(max_lin, total - min_sub);
}

int min_ops(int a, int b) {
    if (a == b) return 0;
    constexpr int LIMIT = 200000;
    std::vector<int> dist(LIMIT + 1, -1);
    std::vector<int> q = {a};
    dist[a] = 0;

    for (int qi = 0; qi < static_cast<int>(q.size()); ++qi) {
        int u = q[qi];
        for (int v : {u - 1, u + 1, u * 2}) {
            if (v < 0 || v > LIMIT) continue;
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + 1;
            if (v == b) return dist[v];
            q.push_back(v);
        }
    }
    return dist[b];
}

int main() {
    assert(min_points_to_pierce({{1, 3}, {2, 5}, {4, 6}}) == 2);
    assert(min_points_to_pierce({{1, 10}, {2, 3}, {4, 5}}) == 2);
    assert(min_points_to_pierce({{1, 5}}) == 1);

    assert(max_circular_subarray({1, -2, 3, -2}) == 3);
    assert(max_circular_subarray({5, -3, 5}) == 10);
    assert(max_circular_subarray({-3, -2, -1}) == -1);
    assert(max_circular_subarray({}) == 0);

    assert(min_ops(2, 3) == 1);
    assert(min_ops(2, 8) == 2);
    assert(min_ops(1, 10) == 4);

    std::print("02-contest-strategy: all assertions passed\n");
    return 0;
}
