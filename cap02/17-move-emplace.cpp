// cap02/17-move-emplace.cpp
// Microbenchmark: copy vs move vs emplace when filling a vector.
// For a heap-owning element type, move and emplace avoid per-element copies;
// for a trivial element type the choice does not matter (a move copies bits).
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o move-emplace 17-move-emplace.cpp

#include <chrono>
#include <print>
#include <string>
#include <utility>
#include <vector>

using Clock = std::chrono::steady_clock;

static double ms_since(Clock::time_point t0) {
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main() {
    const int n = 1'000'000;
    const std::string sample(64, 'x');  // 64-byte payload; owns heap memory

    // (a) copy: each push_back duplicates the 64-byte buffer.
    {
        std::vector<std::string> v;
        v.reserve(n);
        auto t0 = Clock::now();
        for (int i = 0; i < n; ++i) {
            std::string token = sample;   // a fresh lvalue each iteration
            v.push_back(token);           // copy into the vector
        }
        std::println("copy   : {:.1f} ms", ms_since(t0));
    }

    // (b) move: steal each token's buffer instead of copying it.
    {
        std::vector<std::string> v;
        v.reserve(n);
        auto t0 = Clock::now();
        for (int i = 0; i < n; ++i) {
            std::string token = sample;
            v.push_back(std::move(token));  // O(1) pointer steal
        }
        std::println("move   : {:.1f} ms", ms_since(t0));
    }

    // (c) emplace: construct the string directly in the vector's storage.
    {
        std::vector<std::string> v;
        v.reserve(n);
        auto t0 = Clock::now();
        for (int i = 0; i < n; ++i)
            v.emplace_back(64, 'x');        // build in place, no temporary
        std::println("emplace: {:.1f} ms", ms_since(t0));
    }

    // (d) trivial type: moving an int copies the bits, so it equals a copy.
    {
        std::vector<int> v;
        v.reserve(n);
        auto t0 = Clock::now();
        for (int i = 0; i < n; ++i) {
            int x = i;
            v.push_back(std::move(x));      // no cheaper than push_back(x)
        }
        std::println("int    : {:.1f} ms", ms_since(t0));
    }

    return 0;
}
