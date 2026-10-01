// Busca binária com intervalo semiaberto, as versões da biblioteca e as buscas lineares do C++23.
#include <algorithm>
#include <print>
#include <string>
#include <vector>

long long comparacoes = 0;

// Primeira posição i com v[i] >= alvo, ou v.size(). Invariante: v[j] < alvo para j < lo e v[j] >= alvo para j >= hi.
int primeiro_ge(const std::vector<int>& v, int alvo) {
    int lo = 0, hi = static_cast<int>(v.size());     // intervalo desconhecido [lo, hi)
    while (lo < hi) {
        const int meio = lo + (hi - lo) / 2;         // sem transbordamento
        ++comparacoes;
        if (v[meio] < alvo) lo = meio + 1;           // meio é sabidamente falso
        else                hi = meio;               // meio pode ser a resposta
    }
    return lo;
}

// Primeira posição i com v[i] > alvo, ou v.size(). Só a comparação muda.
int primeiro_gt(const std::vector<int>& v, int alvo) {
    int lo = 0, hi = static_cast<int>(v.size());
    while (lo < hi) {
        const int meio = lo + (hi - lo) / 2;
        if (v[meio] <= alvo) lo = meio + 1;
        else                 hi = meio;
    }
    return lo;
}

struct Competidor {
    int id;
    int pontos;
    std::string nome;
};

int main() {
    const std::vector<int> v{1, 2, 2, 2, 5, 8, 9};
    std::println("lower_bound(2) = {}, upper_bound(2) = {}, ocorrências = {}",
                 primeiro_ge(v, 2), primeiro_gt(v, 2), primeiro_gt(v, 2) - primeiro_ge(v, 2));
    std::println("lower_bound(0) = {}, lower_bound(10) = {}, lower_bound(6) = {}",
                 primeiro_ge(v, 0), primeiro_ge(v, 10), primeiro_ge(v, 6));
    const auto a = std::ranges::lower_bound(v, 2), b = std::ranges::upper_bound(v, 2);
    std::println("biblioteca: lower {} upper {} binary_search(7) = {} binary_search(8) = {}",
                 a - v.begin(), b - v.begin(), std::ranges::binary_search(v, 7), std::ranges::binary_search(v, 8));

    // Número de comparações para n = 10^6, no pior caso sobre todos os alvos de 0 a n.
    std::vector<int> grande(1'000'000);
    for (int i = 0; i < 1'000'000; ++i) grande[i] = 2 * i;
    long long pior = 0;
    for (int alvo = -1; alvo <= 2'000'000; alvo += 1) {
        comparacoes = 0;
        primeiro_ge(grande, alvo);
        pior = std::max(pior, comparacoes);
    }
    std::println("n = 10^6: no máximo {} comparações", pior);

    // Projeção: busca pelos pontos em registros ordenados pelos pontos.
    std::vector<Competidor> c{{7, 40, "Ana"}, {3, 55, "Bia"}, {9, 80, "Caio"}, {1, 80, "Davi"}, {4, 95, "Eva"}};
    const auto it = std::ranges::lower_bound(c, 80, {}, &Competidor::pontos);
    std::println("primeiro com pontos >= 80: {} (id {}), existe 95: {}", it->nome, it->id,
                 std::ranges::binary_search(c, 95, {}, &Competidor::pontos));

    // Buscas lineares do C++23, para intervalos não ordenados.
    const std::vector<int> d{4, 9, 1, 9, 3};
    const auto ultimo = std::ranges::find_last(d, 9);
    std::println("contains(d, 3) = {}, contains(d, 7) = {}, find_last(d, 9) na posição {}",
                 std::ranges::contains(d, 3), std::ranges::contains(d, 7), ultimo.begin() - d.begin());
}
