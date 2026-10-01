// Tamanho, capacidade, realocações e reserve no std::vector.
#include <cstdint>
#include <numeric>
#include <print>
#include <vector>
#include "medicao.hpp"

int main() {
    // 1. A sequência de capacidades durante 20 push_back, a partir de um vetor vazio.
    std::vector<int> v;
    std::size_t ultima = v.capacity();
    std::print("capacidades:");
    for (int i = 0; i < 20; ++i) {
        v.push_back(i);
        if (v.capacity() != ultima) {
            ultima = v.capacity();
            std::print(" {}", ultima);
        }
    }
    std::println("");

    // 2. Quantas realocações e quantos elementos movidos para chegar a um milhão.
    std::vector<int> w;
    int realocacoes = 0;
    long long movidos = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        if (w.size() == w.capacity()) {
            ++realocacoes;
            movidos += static_cast<long long>(w.size());
        }
        w.push_back(i);
    }
    std::println("sem reserve: {} realocações, {} elementos movidos, capacidade final {}",
                 realocacoes, movidos, w.capacity());
    std::vector<int> z;
    z.reserve(1'000'000);
    int realoc_z = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        if (z.size() == z.capacity()) ++realoc_z;
        z.push_back(i);
    }
    std::println("com reserve: {} realocações, capacidade final {}", realoc_z, z.capacity());

    // 3. O endereço dos dados muda quando a capacidade muda.
    std::vector<int> a(4, 0);
    a.shrink_to_fit();
    const auto antes = reinterpret_cast<std::uintptr_t>(a.data());
    const auto cap_antes = a.capacity();
    a.push_back(1);
    const auto depois = reinterpret_cast<std::uintptr_t>(a.data());
    std::println("capacidade {} -> {}, endereço mudou: {}", cap_antes, a.capacity(), antes != depois);

    // 4. Tempo de dez milhões de push_back com e sem reserve.
    constexpr int N = 10'000'000;
    auto sem = [] {
        std::vector<int> x;
        for (int i = 0; i < N; ++i) x.push_back(i);
        return x.back() + static_cast<long long>(x.size());
    };
    auto com = [] {
        std::vector<int> x;
        x.reserve(N);
        for (int i = 0; i < N; ++i) x.push_back(i);
        return x.back() + static_cast<long long>(x.size());
    };
    long long obs = 0;
    const Medicao ms = medir(sem, obs);
    const Medicao mc = medir(com, obs);
    std::println("10^7 push_back sem reserve: {:8.3f} ms", ms.mediana_ms);
    std::println("10^7 push_back com reserve: {:8.3f} ms", mc.mediana_ms);

    // 5. iota e accumulate com acumulador largo.
    std::vector<int> b(100'000);
    std::iota(b.begin(), b.end(), 1);
    const long long soma = std::accumulate(b.begin(), b.end(), 0LL);
    std::println("soma de 1 a 100000 = {}", soma);

    // 6. Prefixos em long long a partir de int.
    std::vector<int> c{2'000'000'000, 2'000'000'000, -5, 7};
    std::vector<long long> pref(c.size() + 1, 0);
    for (std::size_t i = 0; i < c.size(); ++i) pref[i + 1] = pref[i] + c[i];
    std::println("prefixos: {} {} {} {} {}", pref[0], pref[1], pref[2], pref[3], pref[4]);
    std::println("soma de [1, 4) = {}", pref[4] - pref[1]);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
