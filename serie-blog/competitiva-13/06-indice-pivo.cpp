// Índice pivô: a menor posição em que a soma à esquerda é igual à soma à direita.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> a(n);
    long long total = 0;                              // até 2 * 10^5 * 10^9 em valor absoluto
    for (auto& x : a) { std::cin >> x; total += x; }
    long long esquerda = 0;
    int pivo = -1;
    for (int i = 0; i < n; ++i) {
        if (esquerda == total - esquerda - a[i]) { pivo = i; break; }   // direita = total - esquerda - a[i]
        esquerda += a[i];
    }
    std::println("{}", pivo);
}
