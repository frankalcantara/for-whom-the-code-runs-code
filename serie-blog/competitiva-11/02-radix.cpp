// Radix sort LSD com dígitos de 8, 11 e 16 bits contra std::ranges::sort, para inteiros de 32 bits.
#include <algorithm>
#include <array>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

// A versão do texto: base 256, quatro passadas, um histograma por passada.
void radix_8(std::vector<std::uint32_t>& a, std::vector<std::uint32_t>& tmp) {
    tmp.resize(a.size());
    for (int passada = 0; passada < 4; ++passada) {
        const int desloc = passada * 8;
        std::array<int, 256> cont{};
        for (auto x : a) ++cont[(x >> desloc) & 0xFFu];
        for (int i = 1; i < 256; ++i) cont[i] += cont[i - 1];
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            const auto x = a[i];
            tmp[--cont[(x >> desloc) & 0xFFu]] = x;
        }
        a.swap(tmp);
    }
}

// Mesma base, com os quatro histogramas calculados em uma única leitura e a distribuição para frente.
void radix_8_um_histograma(std::vector<std::uint32_t>& a, std::vector<std::uint32_t>& tmp) {
    tmp.resize(a.size());
    std::array<std::array<int, 256>, 4> cont{};
    for (auto x : a)
        for (int p = 0; p < 4; ++p) ++cont[p][(x >> (8 * p)) & 0xFFu];
    for (int p = 0; p < 4; ++p) {
        int soma = 0;                                    // início de cada dígito, e não o fim
        for (int i = 0; i < 256; ++i) { const int c = cont[p][i]; cont[p][i] = soma; soma += c; }
    }
    for (int p = 0; p < 4; ++p) {
        const int desloc = 8 * p;
        for (auto x : a) tmp[cont[p][(x >> desloc) & 0xFFu]++] = x;   // para frente, também estável
        a.swap(tmp);
    }
}

// Dígitos de B bits: 11 bits dão três passadas, 16 bits dão duas.
template <int B>
void radix_b(std::vector<std::uint32_t>& a, std::vector<std::uint32_t>& tmp, std::vector<int>& cont) {
    constexpr int base = 1 << B;
    constexpr int passadas = (32 + B - 1) / B;
    constexpr std::uint32_t mascara = base - 1;
    tmp.resize(a.size());
    for (int p = 0; p < passadas; ++p) {
        const int desloc = p * B;
        cont.assign(base, 0);
        for (auto x : a) ++cont[(x >> desloc) & mascara];
        int soma = 0;
        for (int i = 0; i < base; ++i) { const int c = cont[i]; cont[i] = soma; soma += c; }
        for (auto x : a) tmp[cont[(x >> desloc) & mascara]++] = x;
        a.swap(tmp);
    }
}

int main() {
    std::mt19937 gerador(20261001);
    std::uniform_int_distribution<std::uint32_t> dist;
    long long obs = 0;
    std::vector<std::uint32_t> base, v, tmp, ref;
    std::vector<int> cont;
    std::println("{:>9} | {:>10} | {:>10} | {:>12} | {:>10} | {:>10} | {}", "n", "sort", "radix 8",
                 "8, 1 leitura", "radix 11", "radix 16", "iguais");
    for (int n : {10'000, 100'000, 1'000'000, 10'000'000}) {
        base.resize(n);
        for (auto& x : base) x = dist(gerador);
        ref = base;
        std::ranges::sort(ref);
        auto copia = [&] { v = base; };
        bool ok = true;
        const auto s = medir_com_preparo(copia, [&] { std::ranges::sort(v); return v[n / 2]; }, obs);
        const auto r8 = medir_com_preparo(copia, [&] { radix_8(v, tmp); return v[n / 2]; }, obs);
        ok = ok && v == ref;
        const auto r8u = medir_com_preparo(copia, [&] { radix_8_um_histograma(v, tmp); return v[n / 2]; }, obs);
        ok = ok && v == ref;
        const auto r11 = medir_com_preparo(copia, [&] { radix_b<11>(v, tmp, cont); return v[n / 2]; }, obs);
        ok = ok && v == ref;
        const auto r16 = medir_com_preparo(copia, [&] { radix_b<16>(v, tmp, cont); return v[n / 2]; }, obs);
        ok = ok && v == ref;
        std::println("{:>9} | {:>7.3f} ms | {:>7.3f} ms | {:>9.3f} ms | {:>7.3f} ms | {:>7.3f} ms | {}", n,
                     s.mediana_ms, r8.mediana_ms, r8u.mediana_ms, r11.mediana_ms, r16.mediana_ms, ok);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
