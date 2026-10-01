// O cofre dos primos: tabela de primalidade construída em tempo de compilação.
#include <array>
#include <iostream>
#include <print>

constexpr bool eh_primo(int valor) {
    if (valor < 2) return false;
    for (int divisor = 2; divisor <= valor / divisor; ++divisor)   // divisor <= sqrt(valor)
        if (valor % divisor == 0) return false;
    return true;
}

constexpr auto primos = [] {
    std::array<bool, 1000> tabela{};
    for (int v = 0; v < 1000; ++v) tabela[v] = eh_primo(v);
    return tabela;
}();
static_assert(primos[2] && primos[997] && !primos[1] && !primos[999]);

int main() {
    int q = 0;
    std::cin >> q;
    while (q-- > 0) {
        int codigo = 0;
        std::cin >> codigo;
        std::println("{}", primos[codigo] ? "OPEN" : "CLOSED");
    }
}
