// Exercise T06.3 - The Chimney Line
// Session: T06.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 06 (monotonic stack; next greater element to the right)
// Tags: chapter-06, greedy
//
// MODEL: for each i, the smallest j > i with h[j] > h[i], or none.
// MATH: sweep right to left keeping a stack of indices whose heights are
//   strictly decreasing from bottom to top. At chimney i, pop every index
//   with height <= h[i]; the surviving top, if any, is the answer for i;
//   then push i. (The same answers fall out of a left-to-right sweep
//   where i resolves the popped elements instead; one direction per
//   taste, same stack.)
// PROOF: a popped index j (h[j] <= h[i], j > i) can never answer any
//   chimney to the left of i, because i is nearer and at least as tall:
//   i dominates j, so discarding j loses nothing (exchange argument).
//   What remains on the stack is exactly the set of right-side indices
//   not dominated by anything between them and i, in increasing height
//   from top to bottom, so the top is the nearest strictly taller one.
//   Each index is pushed once and popped at most once: 2n stack moves.
// COMPLEXITY: O(n) time, O(n) memory. The naive rightward scan per
//   chimney is O(n^2) = 1e12 on a decreasing row; a sparse table with
//   binary descent gives O(n log n), more code for a worse bound.
// TYPES: heights fit in int; answers are indices, also int. The output
//   is up to 1e6 numbers: buffered output (print to a string, or the
//   '\n' discipline from chapter 03) matters more than any arithmetic.
// ALTERNATIVES: the left-to-right formulation resolving on pop (the one
//   the chapter writes for histogram areas); segment tree descent for
//   the same query online. The stack wins: one pass, no build.
// EDGE CASES: strictly increasing row (every answer is i + 1); strictly
//   decreasing row (every answer 0, maximum stack depth); equal heights
//   (ties are not "taller": answer skips equals); n = 1; the tallest
//   chimney anywhere (always 0).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o chimney t06-s3-1-chimney-line.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fechimney.exe t06-s3-1-chimney-line.cpp

#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<int> h(n);
    for (auto& x : h) std::cin >> x;

    std::vector<int> answer(n, 0);
    std::vector<int> stack;  // indices, heights strictly decreasing
    stack.reserve(n);
    for (int i = n - 1; i >= 0; --i) {
        while (!stack.empty() && h[stack.back()] <= h[i]) stack.pop_back();
        answer[i] = stack.empty() ? 0 : stack.back() + 1;
        stack.push_back(i);
    }

    std::string out;
    out.reserve(static_cast<std::size_t>(n) * 8);
    for (int i = 0; i < n; ++i) {
        out += std::to_string(answer[i]);
        out += (i + 1 < n) ? ' ' : '\n';
    }
    std::print("{}", out);
    return 0;
}
