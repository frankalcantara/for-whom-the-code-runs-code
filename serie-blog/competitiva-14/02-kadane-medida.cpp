// Kadane com o índice de início, que exige um desvio, e Kadane só com a soma, escrito com std::max.
#include <algorithm>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

struct Trecho { long long soma; int l, r; };

Trecho com_indices(const std::vector<int>& a) {
    long long atual = a[0], melhor = a[0];
    int l_atual = 0, l = 0, r = 0;
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        if (atual < 0) { atual = a[i]; l_atual = i; }                   // recomeçar em i rende mais
        else atual += a[i];                                            // estender o melhor trecho que acaba em i - 1
        if (atual > melhor) { melhor = atual; l = l_atual; r = i; }
    }
    return {melhor, l, r};
}

long long so_a_soma(const std::vector<int>& a) {
    long long atual = a[0], melhor = a[0];
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        atual = std::max<long long>(a[i], atual + a[i]);
        melhor = std::max(melhor, atual);
    }
    return melhor;
}

int main() {
    std::mt19937 gerador(20261001);
    const int n = 10'000'000;
    std::vector<int> a(n);
    long long obs = 0;
    auto nada = [] {};
    struct Caso { const char* nome; int lo, hi; };
    for (const Caso c : {Caso{"aleatórios em [-10^9, 10^9]", -1'000'000'000, 1'000'000'000},
                         Caso{"aleatórios em [-10, 9]", -10, 9},
                         Caso{"todos positivos, em [1, 100]", 1, 100}}) {
        std::uniform_int_distribution<int> valor(c.lo, c.hi);
        for (int& x : a) x = valor(gerador);
        Trecho t{};
        long long s = 0;
        const auto m1 = medir_com_preparo(nada, [&] { t = com_indices(a); return t.soma; }, obs);
        const auto m2 = medir_com_preparo(nada, [&] { s = so_a_soma(a); return s; }, obs);
        std::println("{}: com índices {:.2f} ms, só a soma {:.2f} ms, mesma soma: {}", c.nome, m1.mediana_ms,
                     m2.mediana_ms, t.soma == s);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
