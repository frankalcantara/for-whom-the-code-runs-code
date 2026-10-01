// A2.1, o correio de pombos: filtrar e acumular em uma passada, sem guardar o arquivo.
#include <iostream>
#include <print>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string auditada;
    long long n = 0;
    std::cin >> auditada >> n;
    long long tubos = 0, gramas = 0;                  // até 10^6 * 10^9 = 10^15 gramas
    std::string cidade;                               // um único buffer reaproveitado
    for (long long i = 0; i < n; ++i) {
        long long peso = 0;
        std::cin >> cidade >> peso;
        if (cidade == auditada) {
            ++tubos;
            gramas += peso;
        }
    }
    std::println("{} {}", tubos, gramas);
}
