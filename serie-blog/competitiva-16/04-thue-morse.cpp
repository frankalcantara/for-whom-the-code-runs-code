// O teste anti-hash de Thue-Morse: duas cadeias de 1024 letras que colidem módulo 2^64 para qualquer
// base ímpar, e que o módulo 2^61 - 1 com base sorteada separa.
#include <bit>
#include <cstdint>
#include <print>
#include <random>
#include <string>
#include "hash61.hpp"

int main() {
    const int n = 1024;
    std::string s(n, 'a'), t(n, 'a');
    for (int i = 0; i < n; ++i) {
        const bool impar = std::popcount(static_cast<unsigned>(i)) % 2 == 1;   // sequência de Thue-Morse
        s[i] = impar ? 'b' : 'a';
        t[i] = impar ? 'a' : 'b';                                              // o complemento
    }
    auto hash64 = [](const std::string& x, std::uint64_t b) {                 // módulo 2^64, pelo overflow
        std::uint64_t h = 0;
        for (char c : x) h = h * b + static_cast<unsigned char>(c);
        return h;
    };
    auto hash_primo = [](const std::string& x, std::uint64_t b) {
        std::uint64_t h = 0;
        for (char c : x) h = hash61::add(hash61::mul(h, b), static_cast<unsigned char>(c));
        return h;
    };
    std::mt19937_64 gerador(20261016);
    std::uniform_int_distribution<std::uint64_t> sorteio(257, hash61::MOD - 2);
    const int bases = 1000;
    int imp = 0, par = 0, primo = 0;
    for (int k = 0; k < bases; ++k) {
        const std::uint64_t b = gerador();
        if (hash64(s, b | 1) == hash64(t, b | 1)) ++imp;                       // base ímpar
        if (hash64(s, b & ~std::uint64_t{1}) == hash64(t, b & ~std::uint64_t{1})) ++par;
        const std::uint64_t bp = sorteio(gerador);
        if (hash_primo(s, bp) == hash_primo(t, bp)) ++primo;
    }
    std::println("cadeias diferentes: {}", s != t);
    std::println("módulo 2^64, bases ímpares: {} colisões em {}", imp, bases);
    std::println("módulo 2^64, bases pares: {} colisões em {}", par, bases);
    std::println("módulo 2^61 - 1, bases sorteadas: {} colisões em {}", primo, bases);
}
