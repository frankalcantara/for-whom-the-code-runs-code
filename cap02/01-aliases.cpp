// cap02/01-aliases.cpp
// Type aliases, constexpr constants, and lambda utilities
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o aliases 01-aliases.cpp

#include <algorithm>
#include <array>
#include <cstdlib>
#include <print>
#include <ranges>
#include <string>
#include <vector>

// ---- Type aliases used throughout the book ----
// Prefer 'using' over 'typedef': supports template aliases, clearer syntax.
using i32  = int;
using i64  = long long;
using u64  = unsigned long long;
using f64  = double;
using str  = std::string;

using Vi   = std::vector<int>;
using Vl   = std::vector<long long>;
using Vvi  = std::vector<std::vector<int>>;
using Vvl  = std::vector<std::vector<long long>>;
using Vs   = std::vector<std::string>;

// Template alias: Vec<T> instead of std::vector<T>
template <typename T>
using Vec  = std::vector<T>;

// ---- Compile-time constants ----
// Use constexpr, not #define. Scoped, type-safe, debuggable.
constexpr i32 INF   = 2'000'000'000;       // safe sentinel for int comparisons
constexpr i64 LINF  = 4'000'000'000'000'000'000LL;  // sentinel for long long
constexpr f64 EPS   = 1e-9;                // floating-point epsilon

// ---- constexpr functions ----
// Evaluated at compile time when arguments are constants; at runtime otherwise.

constexpr i64 ipow(i64 base, int exp) {    // integer exponentiation — O(exp)
    i64 result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

// Precomputed table: squares[i] = i^2 for i in [0, 20)
// Computed entirely at compile time — zero runtime cost.
constexpr auto make_squares() {
    std::array<i64, 20> arr{};
    for (int i = 0; i < 20; ++i) arr[i] = static_cast<i64>(i) * i;
    return arr;
}
constexpr auto squares = make_squares();

// ---- Lambda utilities ----
// Lambdas capture state and can replace small helper functions inline.

int main() {
    // Basic lambda: sort by absolute value
    Vi v = {-5, 3, -1, 4, -2};
    std::ranges::sort(v, [](i32 a, i32 b) {    // O(n log n)
        return std::abs(a) < std::abs(b);
    });
    std::print("sorted by |x|: ");
    for (auto x : v) std::print("{} ", x);
    std::print("\n");                           // -1 -2 3 4 -5

    // Lambda capturing context: compute partial sums with offset
    i64 offset = 100;
    auto shifted_sum = [&offset](const Vl& a) -> i64 {  // O(n)
        i64 total = 0;
        for (auto x : a) total += x + offset;
        return total;
    };
    Vl data = {1, 2, 3};
    std::print("shifted_sum = {}\n", shifted_sum(data)); // (1+100)+(2+100)+(3+100)=306

    // Compile-time verification of constexpr constants
    static_assert(ipow(2, 10) == 1024, "ipow(2,10) must be 1024");
    static_assert(squares[5] == 25, "5^2 must be 25");
    std::print("ipow(3,5) = {}\n", ipow(3, 5));   // 243
    std::print("squares[7] = {}\n", squares[7]);   // 49
    std::print("INF = {}\n", INF);
    std::print("LINF = {}\n", LINF);
}
