// cap05/06-hashing.cpp
// Prefix hash and rolling hash (Rabin-Karp) for O(1) substring comparison.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o hashing 06-hashing.cpp

#include <cassert>
#include <cstdint>
#include <print>
#include <random>
#include <string>
#include <vector>

// ============================================================
// Polynomial hash.
// h(s) = s[0]*B^(n-1) + s[1]*B^(n-2) + ... + s[n-1]  (mod MOD)
//
// Prefix hash: ph[i] = hash of s[0..i-1], ph[0] = 0.
// Hash of s[l..r): (ph[r] - ph[l] * pw[r-l]) % MOD.
//
// Base B is drawn once at startup from a random source, so an adversary who
// knows the source code cannot craft a collision against a fixed constant
// (the anti-hash test). All PolyHash instances in a run share the same base,
// or hashes from different instances would not be comparable.
// MOD = 2^61 - 1 (Mersenne prime: reduction without division).
// ============================================================
struct PolyHash {
    static constexpr uint64_t MOD = (1ULL << 61) - 1;

    // Single base for the whole program run, chosen on first use.
    static uint64_t base() {
        static const uint64_t B = [] {
            std::mt19937_64 rng(std::random_device{}());
            return std::uniform_int_distribution<uint64_t>(257, MOD - 2)(rng);
        }();
        return B;
    }

    std::vector<uint64_t> ph, pw;  // prefix hashes and powers

    // Safe modular multiplication for MOD = 2^61 - 1.
    static uint64_t mul(uint64_t a, uint64_t b) {
        __uint128_t r = (__uint128_t)a * b;
        uint64_t lo = (uint64_t)(r & MOD);
        uint64_t hi = (uint64_t)(r >> 61);
        uint64_t res = lo + hi;
        if (res >= MOD) res -= MOD;
        return res;
    }

    static uint64_t add(uint64_t a, uint64_t b) {
        a += b;
        if (a >= MOD) a -= MOD;
        return a;
    }

    explicit PolyHash(const std::string& s) {
        int n = static_cast<int>(s.size());
        ph.resize(n + 1, 0);
        pw.resize(n + 1, 1);

        const uint64_t B = base();
        for (int i = 0; i < n; ++i) {
            ph[i + 1] = add(mul(ph[i], B), static_cast<unsigned char>(s[i]));
            pw[i + 1] = mul(pw[i], B);
        }
    }

    // Hash of s[l..r) -- O(1)
    uint64_t query(int l, int r) const {
        uint64_t sub = mul(ph[l], pw[r - l]);
        return (ph[r] + MOD - sub) % MOD;
    }

    // s[l1..r1) == s[l2..r2)? -- O(1)
    bool equal(int l1, int r1, int l2, int r2) const {
        return query(l1, r1) == query(l2, r2);
    }
};

// ============================================================
// Rabin-Karp: pattern search in text -- O(n + m) expected.
// Hash collision triggers an extra O(m) comparison, rare with a good modulus.
// ============================================================
std::vector<int> rabin_karp(const std::string& text, const std::string& pat) {
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pat.size());
    std::vector<int> positions;
    if (m > n) return positions;

    PolyHash ht(text), hp(pat);  // share the same base via PolyHash::base()
    uint64_t ph_pat = hp.query(0, m);

    for (int i = 0; i + m <= n; ++i) {
        if (ht.query(i, i + m) == ph_pat)
            if (text.substr(i, m) == pat)  // anti-collision verification, usually rare
                positions.push_back(i);
    }
    return positions;
}

int main() {
    // s = "abacabacaba" (length 11)
    std::string s = "abacabacaba";
    PolyHash ph(s);

    // s[0..4) = "abac" == s[4..8) = "abac"
    assert(ph.equal(0, 4, 4, 8));    // "abac" == "abac"
    // s[0..3) = "aba"  == s[4..7) = "aba"
    assert(ph.equal(0, 3, 4, 7));    // "aba"  == "aba"
    // s[0..4) = "abac" != s[1..5) = "baca"
    assert(!ph.equal(0, 4, 1, 5));   // "abac" != "baca"

    // Rabin-Karp
    auto pos = rabin_karp("aabaab", "aab");
    assert((pos == std::vector<int>{0, 3}));
    std::print("Rabin-Karp 'aab' in 'aabaab': ");
    for (int p : pos) std::print("{} ", p);
    std::print("\n");

    // LCP via binary search + hash -- O(log n) per query.
    auto lcp = [&](int i, int j) {
        int lo = 0;
        int hi = static_cast<int>(s.size()) - std::max(i, j);
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2;
            if (ph.equal(i, i + mid, j, j + mid)) lo = mid;
            else hi = mid - 1;
        }
        return lo;
    };

    // s[0..] = "abacabacaba", s[4..] = "abacaba" -- LCP = "abacaba" = 7 chars
    assert(lcp(0, 4) == 7);
    std::print("LCP(s[0..], s[4..]): {}\n", lcp(0, 4));

    std::print("06-hashing: all assertions passed\n");
    return 0;
}
