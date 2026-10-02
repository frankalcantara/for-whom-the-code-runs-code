// Exercício 5.6, a expedição equilibrada: dois ponteiros sobre as habilidades ordenadas.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    long long d = 0;
    std::cin >> n >> d;
    std::vector<long long> habilidade(n);
    for (auto& h : habilidade) std::cin >> h;
    std::ranges::sort(habilidade);
    int esquerda = 0, melhor = 0;
    for (int direita = 0; direita < n; ++direita) {
        while (habilidade[direita] - habilidade[esquerda] > d) ++esquerda;   // restaura o contrato
        melhor = std::max(melhor, direita - esquerda + 1);
    }
    std::println("{}", melhor);
}
