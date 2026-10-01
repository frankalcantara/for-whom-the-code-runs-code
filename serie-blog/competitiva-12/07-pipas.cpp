// T04.7, as linhas das pipas: a k-ésima menor distância entre pares, por busca na resposta e contagem.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

// Quantos pares têm distância no máximo x? Para cada i, os parceiros j > i válidos formam um bloco.
long long pares_ate(const std::vector<long long>& a, long long x) {
    long long pares = 0;                              // chega a n(n-1)/2, cerca de 5 * 10^9
    for (std::size_t i = 0; i + 1 < a.size(); ++i) {
        const auto it = std::ranges::upper_bound(a, a[i] + x);   // a[i] + x chega a 2 * 10^9
        pares += (it - a.begin()) - static_cast<long long>(i) - 1;
    }
    return pares;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    long long k = 0;                                  // k chega a cerca de 5 * 10^9
    std::cin >> n >> k;
    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    std::ranges::sort(a);
    long long lo = 0, hi = a[n - 1] - a[0];
    while (lo < hi) {
        const long long meio = lo + (hi - lo) / 2;
        if (pares_ate(a, meio) >= k) hi = meio;       // meio pode ser a resposta
        else lo = meio + 1;
    }
    std::println("{}", lo);
}
