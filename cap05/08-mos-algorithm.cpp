// cap05/08-mos-algorithm.cpp
// Mo's Algorithm -- offline subarray queries in O((n+q) sqrt(n)).
// Problem: for each query [l,r], compute the sum of the elements.
// (Prefix sum would solve this specific aggregate; this file demonstrates the pattern.)
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o mos 08-mos-algorithm.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femos.exe 08-mos-algorithm.cpp
// Test:    printf "5 3\n1 2 3 4 5\n0 2\n1 3\n2 4\n" | ./mos

#include <algorithm>
#include <cmath>
#include <iostream>
#include <print>
#include <vector>

using Vi = std::vector<int>;
using i64 = long long;

struct Query { int l, r, idx; };

// ============================================================
// Mo's Algorithm
// Sorts queries by l-block (block size sqrt(n)) and then by r.
// Serpentine order reduces right-pointer travel.
// The result is O((n+q) sqrt(n)) pointer movement in the usual analysis.
// ============================================================
std::vector<i64> mos_sum(const Vi& a, std::vector<Query>& qs) {
    int n = static_cast<int>(a.size());
    int q = static_cast<int>(qs.size());
    int block = std::max(1, static_cast<int>(std::sqrt(n)));

    // Query ordering: increasing block(l), serpentine r within each block.
    std::sort(qs.begin(), qs.end(), [block](const Query& a, const Query& b) {
        int ba = a.l / block, bb = b.l / block;
        if (ba != bb) return ba < bb;
        return (ba & 1) ? (a.r > b.r) : (a.r < b.r);  // serpentine r order
    });

    std::vector<i64> ans(q);
    int cl = 0, cr = -1;
    i64 cur = 0;

    for (const auto& query : qs) {
        // Expands or contracts [cl, cr] until it matches [query.l, query.r].
        while (cr < query.r) cur += a[++cr];
        while (cl > query.l) cur += a[--cl];
        while (cr > query.r) cur -= a[cr--];
        while (cl < query.l) cur -= a[cl++];
        ans[query.idx] = cur;
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    Vi a(n);
    for (int& x : a) std::cin >> x;

    std::vector<Query> qs(q);
    for (int i = 0; i < q; ++i) {
        std::cin >> qs[i].l >> qs[i].r;
        qs[i].idx = i;
    }

    auto ans = mos_sum(a, qs);
    for (i64 x : ans) std::print("{}\n", x);
    return 0;
}
