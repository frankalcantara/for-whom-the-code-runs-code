// O teste de repetição do Exercício 5.9 com 2 * 10^5 janelas: std::unordered_set contra ordenar
// os pares (hash, início) e comparar vizinhos.
#include <algorithm>
#include <cstdint>
#include <print>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>
#include "medicao.hpp"

int main() {
    std::mt19937_64 gerador(20261016);
    const int n = 200'000;
    std::vector<std::uint64_t> hashes(n);
    for (auto& h : hashes) h = gerador() >> 3;                 // hashes distintos com altíssima probabilidade
    long long obs = 0;
    auto nada = [] {};
    auto com_conjunto = [&] {                                  // pior caso do predicado: nenhuma repetição
        std::unordered_set<std::uint64_t> visto;
        visto.reserve(2 * static_cast<std::size_t>(n));
        int repetidos = 0;
        for (auto h : hashes) if (!visto.insert(h).second) ++repetidos;
        return repetidos;
    };
    auto sem_reserva = [&] {
        std::unordered_set<std::uint64_t> visto;
        int repetidos = 0;
        for (auto h : hashes) if (!visto.insert(h).second) ++repetidos;
        return repetidos;
    };
    std::vector<std::pair<std::uint64_t, int>> pares(n);
    auto ordenando = [&] {
        for (int i = 0; i < n; ++i) pares[i] = {hashes[i], i};
        std::ranges::sort(pares);
        int repetidos = 0;
        for (int i = 1; i < n; ++i) if (pares[i].first == pares[i - 1].first) ++repetidos;
        return repetidos;
    };
    int r1 = 0, r2 = 0, r3 = 0;
    const auto m1 = medir_com_preparo(nada, [&] { r1 = com_conjunto(); return r1; }, obs);
    const auto m2 = medir_com_preparo(nada, [&] { r2 = sem_reserva(); return r2; }, obs);
    const auto m3 = medir_com_preparo(nada, [&] { r3 = ordenando(); return r3; }, obs);
    std::println("2 * 10^5 janelas: unordered_set com reserve {:.2f} ms, sem reserve {:.2f} ms, ordenando os pares {:.2f} ms, mesmas respostas: {}",
                 m1.mediana_ms, m2.mediana_ms, m3.mediana_ms, r1 == r2 && r2 == r3);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
