// Exercício resolvido do ranking: busca na resposta, ordenação dos nomes e upper_bound nas pontuações.
#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

struct Competidor {
    int id;
    int pontos;
    std::string nome;
};

// Maior x com pelo menos k competidores de pontuação >= x.
int pontuacao_k(const std::vector<Competidor>& cs, int k) {
    int lo = 0, hi = 100'000;
    while (lo < hi) {
        const int meio = lo + (hi - lo + 1) / 2;
        const auto quantos = std::ranges::count_if(cs, [meio](const Competidor& c) { return c.pontos >= meio; });
        if (quantos >= k) lo = meio;
        else hi = meio - 1;
    }
    return lo;
}

std::vector<std::string> nomes_com(const std::vector<Competidor>& cs, int alvo) {
    std::vector<std::string> nomes;
    for (const auto& c : cs)
        if (c.pontos == alvo) nomes.push_back(c.nome);
    std::ranges::sort(nomes);
    return nomes;
}

int acima_de(const std::vector<int>& pontuacoes, int limiar) {
    const auto it = std::ranges::upper_bound(pontuacoes, limiar);
    return static_cast<int>(pontuacoes.end() - it);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0, limiar = 0;
    std::cin >> n >> k >> limiar;
    std::vector<Competidor> cs(n);
    for (auto& c : cs) std::cin >> c.id >> c.pontos >> c.nome;

    const int alvo = pontuacao_k(cs, k);
    std::println("{}", alvo);
    std::string linha;
    for (const auto& nome : nomes_com(cs, alvo)) {
        if (!linha.empty()) linha += ' ';
        linha += nome;
    }
    std::println("{}", linha);

    std::vector<int> pontuacoes;
    pontuacoes.reserve(n);
    for (const auto& c : cs) pontuacoes.push_back(c.pontos);
    std::ranges::sort(pontuacoes);
    std::println("{}", acima_de(pontuacoes, limiar));
}
