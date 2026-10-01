// Constantes, funções constexpr, tabelas em tempo de compilação e if consteval.
#include <array>
#include <bit>
#include <cstdint>
#include <print>
#include <vector>

constexpr long long ipow(long long base, int exp) {
    long long resultado = 1;
    for (int i = 0; i < exp; ++i) resultado *= base;
    return resultado;
}
static_assert(ipow(2, 10) == 1024);
static_assert(ipow(10, 18) == 1'000'000'000'000'000'000LL);

constexpr auto quadrados = [] {
    std::array<long long, 20> a{};
    for (int i = 0; i < 20; ++i) a[i] = 1LL * i * i;
    return a;
}();
static_assert(quadrados[19] == 361);

// Vetor temporário durante a avaliação constante; só o inteiro sobrevive.
constexpr int primos_abaixo_de(int n) {
    std::vector<bool> crivo(n, true);
    int total = 0;
    for (int p = 2; p < n; ++p)
        if (crivo[p]) {
            ++total;
            for (int m = 2 * p; m < n; m += p) crivo[m] = false;
        }
    return total;
}
static_assert(primos_abaixo_de(20) == 8);
static_assert(primos_abaixo_de(1000) == 168);

constexpr int zeros_a_esquerda(unsigned x) {
    if consteval {
        int n = 0;
        for (unsigned m = 0x8000'0000u; m && !(x & m); m >>= 1) ++n;
        return n;
    } else {
        return std::countl_zero(x);
    }
}
static_assert(zeros_a_esquerda(1u) == 31);
static_assert(zeros_a_esquerda(0u) == 32);

int main(int argc, char**) {
    // argc impede que o compilador trate estes valores como constantes.
    const unsigned valores[] = {1u, 12u, 40u, 1024u, 0u};
    for (unsigned v : valores) {
        const unsigned x = v + static_cast<unsigned>(argc - 1);
        std::println("x = {:4}: countl_zero = {:2}, zeros_a_esquerda = {:2}, popcount = {}, bit_width = {:2}, "
                     "has_single_bit = {}, bit_floor = {:4}, bit_ceil = {:4}",
                     x, std::countl_zero(x), zeros_a_esquerda(x), std::popcount(x), std::bit_width(x),
                     std::has_single_bit(x), std::bit_floor(x), std::bit_ceil(x));
    }
    std::println("quadrados[12] = {}, primos abaixo de 1000 = {}", quadrados[12], primos_abaixo_de(1000));
}
