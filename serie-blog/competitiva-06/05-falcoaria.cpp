// T02.5, o plantel da falcoaria: ordenação por chave composta com projeção.
#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <tuple>
#include <vector>

struct Falcao {
    std::string nome;
    long long velocidade;
    long long ano;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<Falcao> plantel(n);
    for (auto& [nome, velocidade, ano] : plantel) std::cin >> nome >> velocidade >> ano;

    // Chave (-velocidade, ano, nome): a tupla compara componente a componente.
    // O nome entra por referência constante, sem copiar a string a cada comparação.
    std::ranges::sort(plantel, {}, [](const Falcao& f) {
        return std::tuple<long long, long long, const std::string&>(-f.velocidade, f.ano, f.nome);
    });
    for (const auto& [nome, velocidade, ano] : plantel) std::println("{} {} {}", nome, velocidade, ano);
}
