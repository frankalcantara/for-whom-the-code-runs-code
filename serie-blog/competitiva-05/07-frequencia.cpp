// Análise de frequências com valores limitados a [1, 10^6]: contagem direta.
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    constexpr int V = 1'000'000;
    std::vector<int> freq(V + 1, 0);
    for (int i = 0; i < n; ++i) {
        int x = 0;
        std::cin >> x;
        ++freq[x];
    }
    int moda = 1, unicos = 0;
    for (int v = 1; v <= V; ++v) {
        if (freq[v] > freq[moda]) moda = v;   // estritamente maior: o menor valor vence o empate
        if (freq[v] == 1) ++unicos;
    }
    std::println("{} {}", moda, freq[moda]);
    std::println("{}", unicos);
    for (int v = 1; v <= V; ++v)
        if (freq[v] > 0) std::println("{} {}", v, freq[v]);
}
