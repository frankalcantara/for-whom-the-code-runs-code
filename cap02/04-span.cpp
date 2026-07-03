// cap02/04-span.cpp
// std::span — non-owning views over contiguous memory
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o span 04-span.cpp

#include <array>
#include <print>
#include <span>
#include <vector>

using Vi = std::vector<int>;

// ---- span accepts any contiguous container without copying ----

// Sum elements of any contiguous int range — no copy, no allocation.
// Time: O(n)   Space: O(1)
long long sum_span(std::span<const int> s) {
    long long total = 0;
    for (int x : s) total += x;
    return total;
}

// In-place negation of all elements
void negate_span(std::span<int> s) {           // O(n)
    for (int& x : s) x = -x;
}

// Maximum in a span
int max_span(std::span<const int> s) {         // O(n)
    int best = s[0];
    for (int x : s) best = std::max(best, x);
    return best;
}

// ---- Subspan: a view into part of a sequence ----

// Process columns [c_begin, c_end) of a matrix row as a span.
// Avoids copying the row or creating a temporary vector.
long long partial_row_sum(std::span<const int> row, int c_begin, int c_end) {
    return sum_span(row.subspan(c_begin, c_end - c_begin));  // O(c_end - c_begin)
}

// ---- std::span works with raw arrays, std::array, std::vector ----

int main() {
    // Works with std::vector
    Vi vec = {1, 2, 3, 4, 5};
    std::print("sum(vec) = {}\n", sum_span(vec));            // 15

    // Works with std::array
    std::array<int, 4> arr = {10, 20, 30, 40};
    std::print("max(arr) = {}\n", max_span(arr));            // 40

    // Works with raw array (C array)
    int raw[] = {5, 6, 7, 8};
    std::print("sum(raw) = {}\n", sum_span(raw));            // 26

    // Subspan: view into elements [1, 4)
    std::print("sum(vec[1..4]) = {}\n",
               sum_span(std::span<const int>(vec).subspan(1, 3)));  // 2+3+4 = 9

    // In-place modification
    Vi v2 = {1, -2, 3, -4};
    negate_span(v2);
    std::print("negated: ");
    for (auto x : v2) std::print("{} ", x);                 // -1 2 -3 4
    std::print("\n");

    // partial_row_sum example
    Vi row = {100, 200, 300, 400, 500};
    std::print("row[1..3] sum = {}\n", partial_row_sum(row, 1, 3));  // 200+300 = 500
}
