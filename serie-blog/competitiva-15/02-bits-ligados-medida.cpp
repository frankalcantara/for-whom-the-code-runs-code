// Percorrer os bits ligados de 10^6 máscaras: testar as 64 posições contra uma iteração por bit ligado.
#include <bit>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

long long testando_todas(const std::vector<std::uint64_t>& v) {
    long long s = 0;
    for (std::uint64_t m : v)
        for (int i = 0; i < 64; ++i)
            if ((m >> i) & 1) s += i;
    return s;
}

long long por_bit_ligado(const std::vector<std::uint64_t>& v) {
    long long s = 0;
    for (std::uint64_t m : v)
        while (m != 0) {
            s += std::countr_zero(m);
            m &= m - 1;
        }
    return s;
}

int main() {
    std::mt19937_64 gerador(20261015);
    const int n = 1'000'000;
    std::vector<std::uint64_t> v(n);
    long long obs = 0;
    auto nada = [] {};
    for (int ligados : {1, 8, 32, 63}) {
        std::uniform_int_distribution<int> pos(0, 63);
        for (auto& m : v) {
            m = 0;
            while (std::popcount(m) < ligados) m |= std::uint64_t{1} << pos(gerador);
        }
        long long a = 0, b = 0;
        const auto m1 = medir_com_preparo(nada, [&] { a = testando_todas(v); return a; }, obs);
        const auto m2 = medir_com_preparo(nada, [&] { b = por_bit_ligado(v); return b; }, obs);
        std::println("{} bits ligados: testando as 64 posições {:.2f} ms, por bit ligado {:.2f} ms, mesma soma: {}", ligados,
                     m1.mediana_ms, m2.mediana_ms, a == b);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
