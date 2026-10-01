// Seleção: ordenar tudo, nth_element e partial_sort para a mediana e para os k menores.
#include <algorithm>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

int main() {
    const int n = 10'000'000;
    std::mt19937 gerador(20260930);
    std::uniform_int_distribution<int> dist(0, 2'000'000'000);
    std::vector<int> base(n), v;
    for (int& x : base) x = dist(gerador);
    long long obs = 0;
    auto copia = [&] { v = base; };

    // Referência: a ordem completa.
    std::vector<int> ordenado = base;
    std::ranges::sort(ordenado);

    const auto tudo = medir_com_preparo(copia, [&] { std::ranges::sort(v); return v[n / 2]; }, obs);
    const auto mediana = medir_com_preparo(copia, [&] {
        std::ranges::nth_element(v, v.begin() + n / 2);
        return v[n / 2];
    }, obs);
    const bool ok_mediana = v[n / 2] == ordenado[n / 2]
        && std::ranges::all_of(v.begin(), v.begin() + n / 2, [&](int x) { return x <= v[n / 2]; })
        && std::ranges::all_of(v.begin() + n / 2, v.end(), [&](int x) { return x >= v[n / 2]; });
    std::println("n = 10^7: sort {:.1f} ms, nth_element da mediana {:.1f} ms, mediana correta: {}",
                 tudo.mediana_ms, mediana.mediana_ms, ok_mediana);

    for (int k : {100, 1'000'000}) {
        const auto sel = medir_com_preparo(copia, [&] {
            std::ranges::nth_element(v, v.begin() + (k - 1));
            std::ranges::sort(v.begin(), v.begin() + k);
            return v[k - 1];
        }, obs);
        const bool ok1 = std::ranges::equal(v.begin(), v.begin() + k, ordenado.begin(), ordenado.begin() + k);
        const auto par = medir_com_preparo(copia, [&] {
            std::ranges::partial_sort(v, v.begin() + k);
            return v[k - 1];
        }, obs);
        const bool ok2 = std::ranges::equal(v.begin(), v.begin() + k, ordenado.begin(), ordenado.begin() + k);
        std::println("k = {}: nth_element e sort dos k {:.1f} ms, partial_sort {:.1f} ms, prefixos corretos: {}", k,
                     sel.mediana_ms, par.mediana_ms, ok1 && ok2);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
