// cap07/13-generator-combinations.cpp
// Lazy generation with a C++23 coroutine. combinations(n, k) yields each
// k-combination of {0, ..., n-1} in lexicographic order, one at a time, using
// O(k) memory regardless of how many combinations exist. The caller can stop
// early: nothing past the consumed combinations is ever computed.
//
// Requires <generator> (C++23). In this repository, compiling.sh validates
// this example through the MSVC exception path when it is available; the
// LLVM/libc++ path does not cover <generator> in the same way. The portable
// fallback is a recursive function taking a callback, shown in the book text.

#include <generator>
#include <print>
#include <span>
#include <vector>

// One coroutine, no recursion: advance the combination index vector in place
// and co_yield a view of it each step.
std::generator<std::span<const int>> combinations(int n, int k) {
    std::vector<int> c(k);
    for (int i = 0; i < k; ++i) c[i] = i;
    while (true) {
        co_yield std::span<const int>(c);
        int i = k - 1;
        while (i >= 0 && c[i] == n - k + i) --i;   // find rightmost movable index
        if (i < 0) co_return;                       // last combination emitted
        ++c[i];
        for (int j = i + 1; j < k; ++j) c[j] = c[j - 1] + 1;
    }
}

int main() {
    // Print the first few 3-combinations of {0..5}.
    int shown = 0;
    for (std::span<const int> comb : combinations(6, 3)) {
        std::print("{{");
        for (int i = 0; i < static_cast<int>(comb.size()); ++i)
            std::print("{}{}", (i ? "," : ""), comb[i]);
        std::print("}} ");
        if (++shown == 5) break;        // early stop: the rest is never generated
    }
    std::print("\n");

    // Lazy search: the first 3-combination of {0..9} whose values sum to >= 20.
    for (std::span<const int> comb : combinations(10, 3)) {
        int s = 0;
        for (int v : comb) s += v;
        if (s >= 20) {
            std::println("first combination summing to >= 20: {} {} {}",
                         comb[0], comb[1], comb[2]);
            break;                      // generation halts here
        }
    }
    return 0;
}
