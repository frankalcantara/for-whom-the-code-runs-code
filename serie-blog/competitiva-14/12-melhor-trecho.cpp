// O livro-caixa da feira: o trecho de maior soma, por Kadane, com o fim mais cedo e, nele, o início mais cedo.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    long long atual = a[0], melhor = a[0];            // atual: melhor soma de um trecho que termina em i
    int inicio_atual = 0, l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (atual < 0) { atual = a[i]; inicio_atual = i; }   // um prefixo negativo só atrapalha
        else atual += a[i];                                  // com atual >= 0, estender nunca perde
        if (atual > melhor) { melhor = atual; l = inicio_atual; r = i; }
    }
    std::println("{} {} {}", melhor, l + 1, r + 1);
}
