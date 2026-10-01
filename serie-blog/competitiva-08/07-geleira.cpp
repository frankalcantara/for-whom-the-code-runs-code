// T03.7, o medidor da geleira: divisão euclidiana do total, com resto em [0, n).
#include <print>
#include "leitor.hpp"

int main() {
    static LeitorRapido entrada;                     // 10^7 leituras: std::cin levaria segundos no MSVC
    const long long n = entrada.ler();
    long long total = 0;                             // |total| até 10^7 * 10^9 = 10^16 > 2^53
    for (long long i = 0; i < n; ++i) total += entrada.ler();
    long long g = total / n;                         // o C++ trunca em direção a zero
    long long r = total % n;                         // e o resto tem o sinal do dividendo
    if (r < 0) { r += n; g -= 1; }                   // (g - 1) * n + (r + n) = g * n + r
    std::println("{} {}", g, r);
}
