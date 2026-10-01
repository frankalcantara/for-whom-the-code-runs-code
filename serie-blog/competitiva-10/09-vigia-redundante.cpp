// Exercício 4.5, a vigia redundante: início crescente, fim decrescente nos empates.
#include <algorithm>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<std::pair<long long, long long>> vigias(n);      // [inicio, fim)
    for (auto& [inicio, fim] : vigias) std::cin >> inicio >> fim;
    std::ranges::sort(vigias, [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second > b.second;                              // o mais longo primeiro
    });
    long long maior_fim = vigias.front().second;
    bool redundante = false;
    for (int i = 1; i < n; ++i) {
        if (vigias[i].second <= maior_fim) { redundante = true; break; }
        maior_fim = vigias[i].second;
    }
    std::println("{}", redundante ? "YES" : "NO");
}
