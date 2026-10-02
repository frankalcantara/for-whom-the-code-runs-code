// Somas de todos os 2^22 subconjuntos: n operações por máscara contra uma soma por máscara reaproveitando a anterior.
#include <bit>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

constexpr int n = 22;

long long por_bits(const std::vector<long long>& w, long long alvo) {
    long long c = 0;
    for (std::uint32_t m = 0; m < (std::uint32_t{1} << n); ++m) {
        long long s = 0;
        for (int i = 0; i < n; ++i)
            if ((m >> i) & 1) s += w[i];
        if (s == alvo) ++c;
    }
    return c;
}

long long reaproveitando(const std::vector<long long>& w, long long alvo, std::vector<long long>& soma) {
    long long c = (alvo == 0) ? 1 : 0;
    soma[0] = 0;
    for (std::uint32_t m = 1; m < (std::uint32_t{1} << n); ++m) {
        soma[m] = soma[m & (m - 1)] + w[std::countr_zero(m)];
        if (soma[m] == alvo) ++c;
    }
    return c;
}

int main() {
    std::mt19937_64 gerador(20261015);
    std::uniform_int_distribution<long long> valor(1, 1000);
    std::vector<long long> w(n);
    for (auto& x : w) x = valor(gerador);
    const long long alvo = (w[0] + w[3] + w[7] + w[11] + w[19]);
    std::vector<long long> soma(std::size_t{1} << n);
    long long obs = 0, a = 0, b = 0;
    auto nada = [] {};
    const auto m1 = medir_com_preparo(nada, [&] { a = por_bits(w, alvo); return a; }, obs);
    const auto m2 = medir_com_preparo(nada, [&] { b = reaproveitando(w, alvo, soma); return b; }, obs);
    std::println("2^{} subconjuntos: {} operações por máscara {:.2f} ms, reaproveitando {:.2f} ms, mesma contagem: {}", n, n,
                 m1.mediana_ms, m2.mediana_ms, a == b);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
