// A cada três passos: laço com passo 3 e a forma com enumerate | stride.
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> valores(n);
    for (auto& x : valores) std::cin >> x;
    for (int indice = 0; indice < n; indice += 3)
        std::println("{} {}", indice, valores[indice]);
    // A mesma travessia como vista, conferida contra o laço sem imprimir de novo.
    long long conferidos = 0;
    for (auto [indice, valor] : std::views::enumerate(valores) | std::views::stride(3))
        if (indice % 3 == 0 && valor == valores[static_cast<std::size_t>(indice)]) ++conferidos;
    if (conferidos != (n + 2) / 3) std::println("divergência entre o laço e a vista");
}
