// Exercise A2.2 - The Windmill Quota
// Session: A2, exercise 2 of 4 (5 hours for the evaluation)
// Theme: chapter 04 (binary search on answers, minimization form, ceiling
//   arithmetic without overflow)
// Tags: chapter-04, binary-search-answer, binary-search
//
// MODEL: minimize integer r >= 1 subject to
//   sum_i ceil(h_i / r) <= H.
// MATH: raising r can only shrink each ceiling, so the predicate
//   fits(r) = "total hours at rate r is within H" is monotone:
//   false...false true...true. The answer is the first true. The integer
//   ceiling is computed as (h + r - 1) / r; with h, r up to 1e18 the sum
//   h + r - 1 can overflow, so the program uses the equivalent
//   h / r + (h % r != 0), which never leaves the input's range.
// PROOF: (monotonicity) r' > r implies ceil(h/r') <= ceil(h/r) for each
//   heap, so the sum is non-increasing in r. (bounds) r = max h_i grinds
//   each heap in one hour, n hours total, and n <= H by the constraints,
//   so a true exists; r = 1 may be false. The minimization-form loop
//   (mid rounds down, true narrows hi) converges to the first true, the
//   smallest sufficient rate.
// PROOF detail worth a sentence: the sum of up to 2e5 ceilings, each up
//   to 1e18, can overflow long long MID-CHECK; the predicate therefore
//   short-circuits as soon as the partial sum exceeds H, which also makes
//   hopeless small rates cheap to reject.
// COMPLEXITY: O(n log(max h)) ~ 2e5 * 60 = 1.2e7 cheap operations; O(n)
//   memory.
// TYPES: heaps, rate, hours: long long throughout; the early exit keeps
//   the accumulator below H + ceil(max) at all times, hence representable.
// ALTERNATIVES: trying rates upward from 1 is O(max h) probes; solving
//   each heap's rate in closed form does not compose, because the budget
//   couples the heaps. The answer-space search decouples them per probe.
// EDGE CASES: H large enough that r = 1 works; H = n (rate must be
//   max h_i); a single heap; heaps of size 1; the overflow-bait ceiling.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o windmill a2-02-windmill-quota.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fewindmill.exe a2-02-windmill-quota.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long hours = 0;
    std::cin >> n >> hours;

    std::vector<long long> heap(n);
    for (auto& h : heap) std::cin >> h;

    auto fits = [&](long long r) {
        long long total = 0;
        for (long long h : heap) {
            total += h / r + (h % r != 0 ? 1 : 0);
            if (total > hours) return false;  // also prevents overflow
        }
        return true;
    };

    long long lo = 1;
    long long hi = *std::ranges::max_element(heap);  // n hours <= H: true
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (fits(mid)) hi = mid;
        else           lo = mid + 1;
    }

    std::print("{}\n", lo);
    return 0;
}
