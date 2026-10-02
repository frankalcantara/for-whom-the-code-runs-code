// O k-ésimo menor com muitos valores repetidos: seleção rápida com pivô sorteado e partição em três
// partes, a bandeira holandesa de Dijkstra.
#include <iostream>
#include <print>
#include <random>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;                                        // k contado a partir de 1
    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    std::mt19937_64 gerador(std::random_device{}());
    int lo = 0, hi = n - 1;
    const int alvo = k - 1;
    while (true) {
        const long long pivo = a[std::uniform_int_distribution<int>(lo, hi)(gerador)];
        // invariante: a[lo, menor) < pivo, a[menor, i) == pivo, a[i, maior] por ver, a(maior, hi] > pivo
        int menor = lo, i = lo, maior = hi;
        while (i <= maior) {
            if (a[i] < pivo) std::swap(a[menor++], a[i++]);
            else if (a[i] > pivo) std::swap(a[i], a[maior--]);
            else ++i;
        }
        if (alvo < menor) hi = menor - 1;                      // está entre os menores
        else if (alvo > maior) lo = maior + 1;                 // está entre os maiores
        else { std::println("{}", pivo); break; }              // está no bloco dos iguais
    }
}
