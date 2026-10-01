// T04.6, o saltério girado: busca binária pelo primeiro salmo que não passa do último.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> salmo(n);
    for (auto& s : salmo) std::cin >> s;
    const long long ultimo = salmo[n - 1];
    int lo = 0, hi = n - 1;                           // salmo[n - 1] <= ultimo: existe um verdadeiro
    while (lo < hi) {
        const int meio = lo + (hi - lo) / 2;
        if (salmo[meio] <= ultimo) hi = meio;         // meio já está na corrida final
        else lo = meio + 1;                           // meio é uma página movida
    }
    std::println("{}", lo);
}
