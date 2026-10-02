// Janela de tamanho fixo: a primeira posição em que k elementos consecutivos somam o alvo.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    long long alvo = 0;
    std::cin >> n >> k >> alvo;
    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    int resposta = -1;
    if (k <= n) {
        long long janela = 0;                         // até 2 * 10^5 * 10^9 em valor absoluto
        for (int i = 0; i < k; ++i) janela += a[i];
        if (janela == alvo) resposta = 1;
        for (int i = k; i < n && resposta == -1; ++i) {
            janela += a[i] - a[i - k];                // entra a[i], sai a[i - k]
            if (janela == alvo) resposta = i - k + 2; // início contado a partir de 1
        }
    }
    std::println("{}", resposta);
}
