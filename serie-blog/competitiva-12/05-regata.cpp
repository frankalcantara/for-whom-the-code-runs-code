// T04.5, a classificação da regata: stable_sort por pontos decrescentes preserva o quadro anterior.
#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

struct Barco {
    std::string nome;
    long long pontos;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<Barco> quadro(n);
    for (auto& b : quadro) std::cin >> b.nome >> b.pontos;
    std::ranges::stable_sort(quadro, std::greater<>{}, &Barco::pontos);
    std::string saida;
    for (const auto& b : quadro) {
        saida += b.nome;
        saida.push_back('\n');
    }
    std::print("{}", saida);
}
