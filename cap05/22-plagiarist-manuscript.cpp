// cap05/22-plagiarist-manuscript.cpp
// Longest substring that occurs at least twice (overlaps allowed),
// solved by binary search on the length plus prefix hashing with a base
// drawn at random so no adversarial input can target a fixed constant.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o plagiarist 22-plagiarist-manuscript.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

struct RollingHash {
    static constexpr std::uint64_t MOD = (1ULL << 61) - 1;
    std::uint64_t base;
    std::vector<std::uint64_t> ph, pw;

    static std::uint64_t mul(std::uint64_t a, std::uint64_t b) {
        __uint128_t r = static_cast<__uint128_t>(a) * b;
        std::uint64_t lo = static_cast<std::uint64_t>(r & MOD);
        std::uint64_t hi = static_cast<std::uint64_t>(r >> 61);
        std::uint64_t res = lo + hi;
        if (res >= MOD) res -= MOD;
        return res;
    }

    RollingHash(const std::string& s, std::uint64_t b) : base(b) {
        int n = static_cast<int>(s.size());
        ph.assign(n + 1, 0);
        pw.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            ph[i + 1] = mul(ph[i], base) + static_cast<unsigned char>(s[i]);
            if (ph[i + 1] >= MOD) ph[i + 1] -= MOD;
            pw[i + 1] = mul(pw[i], base);
        }
    }

    std::uint64_t query(int l, int r) const {  // hash of s[l..r)
        std::uint64_t sub = mul(ph[l], pw[r - l]);
        return (ph[r] + MOD - sub) % MOD;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    if (!(std::cin >> n)) return 0;
    std::string s;
    std::cin >> s;

    std::mt19937_64 rng(std::random_device{}());
    std::uint64_t base =
        std::uniform_int_distribution<std::uint64_t>(257, RollingHash::MOD - 2)(rng);
    RollingHash h(s, base);

    // A length L "works" if two windows of length L share a hash. Lengths form
    // a monotone predicate: if a block of length L repeats, its length-(L-1)
    // prefix repeats too, so binary search finds the largest L that works.
    auto repeats = [&](int L) -> bool {
        if (L == 0) return true;
        std::unordered_set<std::uint64_t> seen;
        seen.reserve(static_cast<std::size_t>(n - L + 1) * 2);
        for (int i = 0; i + L <= n; ++i)
            if (!seen.insert(h.query(i, i + L)).second) return true;
        return false;
    };

    int lo = 0, hi = (n > 0 ? n - 1 : 0);  // overlaps allowed, so at most n-1
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        if (repeats(mid)) lo = mid;
        else hi = mid - 1;
    }
    std::println("{}", lo);
    return 0;
}
