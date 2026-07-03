// cap05/24-bit-cast-byteswap.cpp
// std::bit_cast and std::byteswap. bit_cast reinterprets the object
// representation of a value as another type of the same size, with no
// undefined behavior and no reinterpret_cast aliasing trap; byteswap reverses
// byte order, the usual need when binary input arrives big-endian on a
// little-endian host.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o bitcast 24-bit-cast-byteswap.cpp

#include <bit>
#include <cassert>
#include <cstdint>
#include <print>
#include <unordered_set>
#include <vector>

// Portable byteswap: std::byteswap is C++23; fall back to a builtin when the
// standard library does not ship it yet.
static constexpr std::uint32_t bswap32(std::uint32_t x) {
#if defined(__cpp_lib_byteswap)
    return std::byteswap(x);
#else
    return __builtin_bswap32(x);
#endif
}

int main() {
    // bit_cast exposes the exact 64-bit pattern of a double.
    double a = 0.1 + 0.2;   // not exactly 0.3 in binary floating point
    double b = 0.3;
    auto ba = std::bit_cast<std::uint64_t>(a);
    auto bb = std::bit_cast<std::uint64_t>(b);
    std::println("0.1+0.2 bits: {}", ba);
    std::println("0.3     bits: {}", bb);
    assert(ba != bb);       // they really differ, bit for bit

    // +0.0 and -0.0 compare equal yet have different representations.
    auto pz = std::bit_cast<std::uint64_t>(+0.0);
    auto nz = std::bit_cast<std::uint64_t>(-0.0);
    assert((+0.0 == -0.0) && pz != nz);

    // Dedup doubles by exact value, using the bit pattern as the key.
    std::vector<double> xs{1.5, 1.5, 2.25, 2.25, 2.25, 3.0};
    std::unordered_set<std::uint64_t> seen;
    for (double x : xs) seen.insert(std::bit_cast<std::uint64_t>(x));
    std::println("distinct doubles: {}", seen.size());
    assert(seen.size() == 3);

    // A big-endian 32-bit value read on a little-endian host needs a swap.
    std::uint32_t be = 0x01020304u;
    std::uint32_t host = bswap32(be);
    std::println("0x{:08x} -> 0x{:08x}", be, host);
    assert(host == 0x04030201u);

    std::println("24-bit-cast-byteswap: all assertions passed");
    return 0;
}
