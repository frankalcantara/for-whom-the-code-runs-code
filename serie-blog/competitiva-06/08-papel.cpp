// A1.1, a cota da fábrica de papel: dividir antes de multiplicar.
#include <iostream>
#include <print>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    long long turno = 0;
    int m = 0;
    std::cin >> turno >> m;
    long long total = 0;
    for (int i = 0; i < m; ++i) {
        long long folhas = 0, ciclo = 0;
        std::cin >> folhas >> ciclo;
        total += folhas * (turno / ciclo);           // folhas * turno chegaria a 10^27
    }
    std::println("{}", total);
}
