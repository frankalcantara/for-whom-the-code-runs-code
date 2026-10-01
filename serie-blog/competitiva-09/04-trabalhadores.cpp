// Dividir tarefas consecutivas entre k trabalhadores, minimizando a maior carga: busca na resposta.
#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <vector>

// Com limite fixo, o guloso usa o menor número possível de trabalhadores.
bool viavel(const std::vector<long long>& t, int k, long long limite) {
    int trabalhadores = 1;
    long long atual = 0;
    for (long long x : t) {
        if (x > limite) return false;
        if (atual + x > limite) { ++trabalhadores; atual = 0; }
        atual += x;
        if (trabalhadores > k) return false;
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<long long> t(n);
    for (auto& x : t) std::cin >> x;
    long long lo = *std::ranges::max_element(t);                 // alguém faz a maior tarefa
    long long hi = std::accumulate(t.begin(), t.end(), 0LL);    // um trabalhador faz tudo
    while (lo < hi) {                                            // primeiro verdadeiro em [lo, hi]
        const long long meio = lo + (hi - lo) / 2;
        if (viavel(t, k, meio)) hi = meio;                       // meio serve: tente menor
        else                    lo = meio + 1;                   // meio não serve: precisa de mais
    }
    std::println("{}", lo);
}
