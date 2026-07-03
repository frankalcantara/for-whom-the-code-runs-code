// cap02/19-ranges-demo.cpp
// Ranges in idiomatic C++23. The pipeline materializes with std::ranges::to;
// std::views::enumerate and std::views::zip replace manual index loops. The
// portable fallback for each is noted in a comment. Requires a C++23 ranges
// implementation (libc++ or a recent libstdc++); GCC 11 lacks to/enumerate/zip.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o ranges-demo 19-ranges-demo.cpp

#include <cassert>
#include <print>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Even numbers, squared, first three, materialized into an owning vector.
    // Fallback: a push_back loop over the same lazy pipeline.
    auto got = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3)
        | std::ranges::to<std::vector>();
    assert((got == std::vector<int>{4, 16, 36}));

    // enumerate: index and value together.
    // Fallback: for (int i = 0; i < (int)v.size(); ++i) use(i, v[i]);
    long long index_weighted = 0;
    for (auto [i, x] : std::views::enumerate(v))
        index_weighted += i * x;
    assert(index_weighted == 0*1 + 1*2 + 2*3 + 3*4 + 4*5 + 5*6 + 6*7 + 7*8 + 8*9 + 9*10);

    // zip: advance two ranges together.
    // Fallback: for (int i = 0; i < (int)xs.size(); ++i) use(xs[i], ys[i]);
    std::vector<int> xs{1, 2, 3}, ys{10, 20, 30};
    long long dot = 0;
    for (auto [a, b] : std::views::zip(xs, ys))
        dot += static_cast<long long>(a) * b;
    assert(dot == 140);

    std::println("pipe = {} {} {}", got[0], got[1], got[2]);
    std::println("index_weighted = {}", index_weighted);
    std::println("dot = {}", dot);
    std::println("19-ranges-demo: all assertions passed");
    return 0;
}
