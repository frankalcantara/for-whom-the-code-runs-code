// cap05/23-apothecary-shelves.cpp
// Multiset equality of two subarrays, answered in O(1) per query by giving
// each distinct value a random 64-bit fingerprint and taking prefix sums of
// those fingerprints. Two segments hold the same multiset iff their fingerprint
// sums (and lengths) match; a random tag makes a false match a ~2^-64 event.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o shelves 23-apothecary-shelves.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feshelves.exe 23-apothecary-shelves.cpp

#include <cstdint>
#include <iostream>
#include <print>
#include <random>
#include <unordered_map>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int na = 0, nb = 0, q = 0;
    if (!(std::cin >> na >> nb >> q)) return 0;
    std::vector<int> a(na), b(nb);
    for (auto& x : a) std::cin >> x;
    for (auto& x : b) std::cin >> x;

    std::mt19937_64 rng(std::random_device{}());
    std::unordered_map<int, std::uint64_t> tag;
    auto tag_of = [&](int v) -> std::uint64_t {
        auto it = tag.find(v);
        if (it != tag.end()) return it->second;
        std::uint64_t t = rng();  // one random fingerprint per distinct value
        tag.emplace(v, t);
        return t;
    };

    // Prefix sums of fingerprints. Overflow wraps modulo 2^64 by design: the
    // sum is itself the hash, and unsigned wraparound is well defined.
    std::vector<std::uint64_t> pa(na + 1, 0), pb(nb + 1, 0);
    for (int i = 0; i < na; ++i) pa[i + 1] = pa[i] + tag_of(a[i]);
    for (int i = 0; i < nb; ++i) pb[i + 1] = pb[i] + tag_of(b[i]);

    while (q--) {
        int l1 = 0, r1 = 0, l2 = 0, r2 = 0;
        std::cin >> l1 >> r1 >> l2 >> r2;  // 1-based, inclusive
        std::uint64_t fa = pa[r1] - pa[l1 - 1];
        std::uint64_t fb = pb[r2] - pb[l2 - 1];
        bool same = (r1 - l1) == (r2 - l2) && fa == fb;
        std::println("{}", same ? "YES" : "NO");
    }
    return 0;
}
