// cap03/17-expected-monadic.cpp
// Composing fallible steps with std::expected in idiomatic C++23: the monadic
// pipeline and_then / transform, with value_or for a default. The portable
// early-return staircase is kept in a comment as the fallback for standard
// libraries without the monadic methods.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o expected-monadic 17-expected-monadic.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feexpected-monadic.exe 17-expected-monadic.cpp

#include <cassert>
#include <expected>
#include <print>
#include <string>
#include <string_view>

using Err = std::string;

std::expected<int, Err> to_int(std::string_view s) {
    if (s.empty()) return std::unexpected("empty");
    int v = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::unexpected("not a digit");
        v = v * 10 + (c - '0');
    }
    return v;
}

std::expected<int, Err> reciprocal_scaled(int x) {
    if (x == 0) return std::unexpected("division by zero");
    return 1000 / x;
}

// Idiomatic C++23: the chain short-circuits on the first error.
std::expected<int, Err> pipeline(std::string_view s) {
    return to_int(s)
        .and_then(reciprocal_scaled)
        .transform([](int x) { return x + 1; });
}
// Portable fallback (same meaning), where monadic methods are unavailable:
//   auto a = to_int(s);            if (!a) return std::unexpected(a.error());
//   auto b = reciprocal_scaled(*a); if (!b) return std::unexpected(b.error());
//   return *b + 1;

int main() {
    assert(pipeline("8").value() == 1000 / 8 + 1);
    assert(!pipeline("0"));     // division by zero, error propagates
    assert(!pipeline("xy"));    // not a digit
    assert(pipeline("0").value_or(-1) == -1);

    std::println("pipeline(\"8\") = {}", pipeline("8").value());
    std::println("pipeline(\"0\") error = {}", pipeline("0").error());
    std::println("17-expected-monadic: all assertions passed");
    return 0;
}
