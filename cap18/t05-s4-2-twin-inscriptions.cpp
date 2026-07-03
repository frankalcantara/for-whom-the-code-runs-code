// Exercise T05.6 - The Twin Inscriptions
// Session: T05.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 05 (prefix hash; O(1) substring equality after O(n) build)
// Tags: chapter-05, modular-arithmetic, string-algorithms, rolling-hash
//
// MODEL: q equality tests between substrings of one string, each test in
//   O(1) after preprocessing, via polynomial rolling hash.
// MATH: H(i) = hash of prefix s[1..i] with H(i) = H(i-1) * B + s[i],
//   all mod M. The hash of s[a..a+len-1] is
//   H(a+len-1) - H(a-1) * B^len  (mod M),
//   because multiplying the shorter prefix by B^len aligns both prefixes
//   at the same positional weights, and subtraction cancels everything
//   before position a.
// PROOF: equal substrings always produce equal hashes (the formula is a
//   function of the characters alone). Unequal substrings collide with
//   probability ~ n/M per comparison; with M ~ 1e18 (two 30-bit-safe
//   factors via __int128, here a single 61-bit Mersenne-like modulus) and
//   2e5 comparisons the expected collisions are ~ 1e-7. The base B is
//   random per run, so no fixed adversarial input defeats it (the
//   anti-hash-test argument from chapter 05).
// COMPLEXITY: O(n) build for prefix hashes and powers of B, O(1) per
//   claim: O(n + q) total, O(n) memory. The honest memcmp per claim is
//   O(len) and reaches 2e5 * 2e5 = 4e10 character reads in the worst case.
// TYPES: the modulus M = 2^61 - 1 requires 128-bit intermediates for the
//   product of two 61-bit residues: unsigned __int128 at the single
//   multiplication site, stored back into uint64_t. Everything else fits
//   in 64 bits by construction.
// ALTERNATIVES: suffix arrays with LCP (chapter 12 material, not yet
//   liberado); Z-function per claim is O(n) each. Double hashing with two
//   moduli would square the collision guarantee at twice the cost; one
//   61-bit modulus with a random base is the standard contest compromise.
// EDGE CASES: a = b (trivially YES, same positions); len = n (whole band
//   against itself); len = 1; overlapping passages; claims at both ends
//   of the band.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o twins t05-s4-2-twin-inscriptions.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr std::uint64_t kMod = (1ULL << 61) - 1;

std::uint64_t mulmod(std::uint64_t a, std::uint64_t b) {
    return static_cast<std::uint64_t>(
        static_cast<unsigned __int128>(a) * b % kMod);
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    int n = static_cast<int>(s.size());

    std::mt19937_64 rng(std::random_device{}());
    std::uint64_t base =
        std::uniform_int_distribution<std::uint64_t>(257, kMod - 2)(rng);

    std::vector<std::uint64_t> h(n + 1, 0);
    std::vector<std::uint64_t> pw(n + 1, 1);
    for (int i = 0; i < n; ++i) {
        h[i + 1] = (mulmod(h[i], base) + static_cast<unsigned char>(s[i])) % kMod;
        pw[i + 1] = mulmod(pw[i], base);
    }

    // hash of s[a..a+len-1], a 1-based
    auto cut = [&](int a, int len) -> std::uint64_t {
        std::uint64_t whole = h[a + len - 1];
        std::uint64_t before = mulmod(h[a - 1], pw[len]);
        return (whole + kMod - before) % kMod;
    };

    int q = 0;
    std::cin >> q;
    while (q-- > 0) {
        int a = 0;
        int b = 0;
        int len = 0;
        std::cin >> a >> b >> len;
        std::print("{}\n", cut(a, len) == cut(b, len) ? "YES" : "NO");
    }
    return 0;
}
