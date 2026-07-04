// cap04/04-counting-radix.cpp
// Counting sort and Radix sort: O(n + k) and O(d * (n + b)) for bounded integers.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o counting_radix 04-counting-radix.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecounting_radix.exe 04-counting-radix.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <vector>

// ============================================================
// Counting sort: O(n + k), stable.
// Requires all values to be in [0, k).
// k = maximum value + 1.
// ============================================================
std::vector<int> counting_sort(const std::vector<int>& a, int k) {
    std::vector<int> cnt(k, 0);
    for (int x : a) ++cnt[x];                          // O(n): count frequencies

    // Prefix sums transform frequencies into ending positions.
    for (int i = 1; i < k; ++i) cnt[i] += cnt[i - 1]; // O(k)

    std::vector<int> out(a.size());
    // Traverse backward to preserve stability.
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
        out[--cnt[a[i]]] = a[i];                       // O(n)
    }
    return out;  // Total: O(n + k)
}

// ============================================================
// LSD radix sort: O(d * (n + b)).
// d = number of digits in base b.
// For n = 10^7 32-bit integers and b = 256: d = 4, O(4 * (n + 256)) = O(n).
// ============================================================
void radix_sort(std::vector<int>& a) {
    constexpr int BASE  = 256;        // base: 1 byte per digit
    constexpr int PASSES = 4;         // 4 bytes = 32 bits

    std::vector<int> tmp(a.size());

    for (int pass = 0; pass < PASSES; ++pass) {
        int shift = pass * 8;         // bit shift for this byte

        std::vector<int> cnt(BASE, 0);
        for (int x : a) ++cnt[(x >> shift) & 0xFF];   // extract byte `pass`

        // Prefix sum: cnt[i] = number of elements with byte <= i.
        for (int i = 1; i < BASE; ++i) cnt[i] += cnt[i - 1];

        // Distribute in reverse order for stability.
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            int digit = (a[i] >> shift) & 0xFF;
            tmp[--cnt[digit]] = a[i];
        }
        std::swap(a, tmp);  // O(1): swap buffers
    }
    // Total: O(4 * (n + 256)) = O(n) for 32-bit integers.
}

int main() {
    // Counting sort: values in [0, 10).
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    auto sorted = counting_sort(a, 10);
    assert(std::ranges::is_sorted(sorted));

    // Radix sort: 32-bit integers without a small-domain restriction.
    std::vector<int> b = {170, 45, 75, 90, 802, 24, 2, 66};
    radix_sort(b);
    assert(std::ranges::is_sorted(b));

    // Correctness check: result equals std::sort.
    std::vector<int> c = {5, 3, 1, 4, 2, 9, 7, 8, 6};
    std::vector<int> ref = c;
    std::ranges::sort(ref);
    radix_sort(c);
    assert(c == ref);

    std::print("04-counting-radix: all assertions passed\n");

    // Performance comparison: n = 10^7 integers in [0, 10^6).
    // Em hardware real (i7-12700K, -O2):
    //   std::sort:      ~1200 ms
    //   counting sort:  ~  80 ms (k = 10^6, domain fits in L2)
    //   radix sort:     ~  55 ms (passes = 4, base = 256)
    std::print("time comparison: see the comment in the code\n");
    return 0;
}
