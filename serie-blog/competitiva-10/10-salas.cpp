// Exercício 4.6, as salas do festival: cada show na menor sala livre que o comporta.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<long long> salas(n), publicos(k);
    for (auto& c : salas) std::cin >> c;
    for (auto& d : publicos) std::cin >> d;
    std::ranges::sort(salas);
    std::ranges::sort(publicos);
    int sala = 0, agendados = 0;
    for (long long publico : publicos) {
        while (sala < n && salas[sala] < publico) ++sala;        // pequena demais para este e para os próximos
        if (sala == n) break;
        ++agendados;
        ++sala;
    }
    std::println("{}", agendados);
}
