// Aritmética módulo 2^61 - 1 para hash polinomial, com o produto de 128 bits
// obtido por _umul128 no MSVC e por unsigned __int128 no GCC e no Clang.
#pragma once
#include <cstdint>
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

namespace hash61 {

inline constexpr std::uint64_t MOD = (std::uint64_t{1} << 61) - 1;

// a, b < MOD. O produto tem até 122 bits: produto = alto * 2^61 + baixo, e 2^61 ≡ 1.
inline std::uint64_t mul(std::uint64_t a, std::uint64_t b) {
#if defined(_MSC_VER) && !defined(__clang__)
    std::uint64_t hi = 0;
    const std::uint64_t lo = _umul128(a, b, &hi);            // produto = hi * 2^64 + lo
    std::uint64_t r = (lo & MOD) + ((lo >> 61) | (hi << 3));  // baixo + alto, ambos menores que 2^61
#else
    const unsigned __int128 p = static_cast<unsigned __int128>(a) * b;
    std::uint64_t r = (static_cast<std::uint64_t>(p) & MOD) + static_cast<std::uint64_t>(p >> 61);
#endif
    if (r >= MOD) r -= MOD;
    return r;
}

inline std::uint64_t add(std::uint64_t a, std::uint64_t b) {
    std::uint64_t r = a + b;
    if (r >= MOD) r -= MOD;
    return r;
}

}  // namespace hash61
