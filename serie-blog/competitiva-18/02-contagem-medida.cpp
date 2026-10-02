// Frequências de 10^7 valores em [0, 10^6): vetor de contagem, std::unordered_map, std::map e
// ordenar e varrer.
#include <algorithm>
#include <map>
#include <print>
#include <random>
#include <unordered_map>
#include <vector>
#include "medicao.hpp"

int main() {
    std::mt19937_64 gerador(20261018);
    const int n = 10'000'000, U = 1'000'000;
    std::vector<int> a(n), copia;
    std::uniform_int_distribution<int> valor(0, U - 1);
    for (auto& x : a) x = valor(gerador);
    long long obs = 0;
    auto nada = [] {};
    // assinatura do resultado: soma de valor * frequência ao quadrado, igual em todas as formas
    long long r1 = 0, r2 = 0, r3 = 0, r4 = 0;
    const auto m1 = medir_com_preparo(nada, [&] {
        std::vector<int> f(U, 0);
        for (int x : a) ++f[x];
        long long s = 0;
        for (int v = 0; v < U; ++v) s += static_cast<long long>(v) * f[v] * f[v];
        return r1 = s;
    }, obs);
    const auto m2 = medir_com_preparo(nada, [&] {
        std::unordered_map<int, int> f;
        f.reserve(U);
        for (int x : a) ++f[x];
        long long s = 0;
        for (const auto& [v, c] : f) s += static_cast<long long>(v) * c * c;
        return r2 = s;
    }, obs);
    const auto m3 = medir_com_preparo<1>(nada, [&] {
        std::map<int, int> f;
        for (int x : a) ++f[x];
        long long s = 0;
        for (const auto& [v, c] : f) s += static_cast<long long>(v) * c * c;
        return r3 = s;
    }, obs);
    const auto m4 = medir_com_preparo([&] { copia = a; }, [&] {
        std::ranges::sort(copia);
        long long s = 0;
        for (std::size_t i = 0; i < copia.size();) {
            std::size_t j = i;
            while (j < copia.size() && copia[j] == copia[i]) ++j;
            const long long c = static_cast<long long>(j - i);
            s += copia[i] * c * c;
            i = j;
        }
        return r4 = s;
    }, obs);
    // referência de sanidade da série: ordenar 10^6 inteiros aleatórios leva de 60 a 70 ms nesta máquina
    std::vector<int> ref(1'000'000), ref_copia;
    std::uniform_int_distribution<int> qualquer;
    for (auto& x : ref) x = qualquer(gerador);
    const auto m0 = medir_com_preparo([&] { ref_copia = ref; }, [&] { std::ranges::sort(ref_copia); return ref_copia[500'000]; }, obs);
    std::println("referência, sort de 10^6 inteiros: {:.1f} ms", m0.mediana_ms);
    std::println("vetor de contagem: {:.1f} ms", m1.mediana_ms);
    std::println("std::unordered_map com reserve: {:.1f} ms", m2.mediana_ms);
    std::println("std::map: {:.1f} ms, uma rodada", m3.mediana_ms);
    std::println("ordenar e varrer: {:.1f} ms", m4.mediana_ms);
    std::println("mesmas frequências: {}", r1 == r2 && r2 == r3 && r3 == r4);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
