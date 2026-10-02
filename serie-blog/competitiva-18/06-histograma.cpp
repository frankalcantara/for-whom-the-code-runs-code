// O maior retângulo de um histograma: uma pilha de alturas crescentes descobre, para cada barra, o
// primeiro menor à esquerda e à direita.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> h(n + 1, 0);                 // h[n] = 0 é uma sentinela que esvazia a pilha
    for (int i = 0; i < n; ++i) std::cin >> h[i];
    std::vector<int> pilha;
    pilha.reserve(n + 1);
    long long melhor = 0;                               // até 2 * 10^5 * 10^9 = 2 * 10^14
    for (int i = 0; i <= n; ++i) {
        while (!pilha.empty() && h[pilha.back()] >= h[i]) {
            const long long altura = h[pilha.back()];
            pilha.pop_back();
            const int esquerda = pilha.empty() ? -1 : pilha.back();     // primeiro menor à esquerda
            melhor = std::max(melhor, altura * (i - esquerda - 1));     // i é o primeiro menor ou igual à direita
        }
        pilha.push_back(i);
    }
    std::println("{}", melhor);
}
