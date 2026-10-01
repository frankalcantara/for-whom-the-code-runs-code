// T02.4, a prateleira da padaria: maior soma de k pães consecutivos.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<long long> prefixo(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long calor = 0;
        std::cin >> calor;
        prefixo[i + 1] = prefixo[i] + calor;
    }
    long long melhor = prefixo[k] - prefixo[0];      // começa pela primeira janela, e não por zero
    for (int inicio = 1; inicio + k <= n; ++inicio)
        if (const long long janela = prefixo[inicio + k] - prefixo[inicio]; janela > melhor) melhor = janela;
    std::println("{}", melhor);
}
