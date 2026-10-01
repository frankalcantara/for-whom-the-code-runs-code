// Instalar k lâmpadas em n postes, maximizando a menor distância entre lâmpadas: o último verdadeiro.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

// Com distância mínima d, o guloso instala o maior número possível de lâmpadas.
bool cabem(const std::vector<long long>& x, int k, long long d) {
    int instaladas = 1;
    long long ultima = x[0];
    for (std::size_t i = 1; i < x.size(); ++i)
        if (x[i] - ultima >= d) {
            ++instaladas;
            ultima = x[i];
            if (instaladas == k) return true;
        }
    return instaladas >= k;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<long long> x(n);
    for (auto& p : x) std::cin >> p;
    std::ranges::sort(x);
    long long lo = 0, hi = x.back() - x.front();     // d = 0 sempre serve
    while (lo < hi) {                                // último verdadeiro em [lo, hi]
        const long long meio = lo + (hi - lo + 1) / 2;   // arredonda para cima
        if (cabem(x, k, meio)) lo = meio;
        else                   hi = meio - 1;
    }
    std::println("{}", lo);
}
