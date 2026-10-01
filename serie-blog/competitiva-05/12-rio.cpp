// O livro do rio: maior soma de trecho contíguo dentro de [l, r], sobre um std::span.
#include <algorithm>
#include <iostream>
#include <print>
#include <span>
#include <vector>

long long melhor_trecho(std::span<const long long> a) {
    long long termina_aqui = a[0];   // E_0: o trecho não pode ser vazio
    long long melhor = a[0];         // B_0
    for (std::size_t i = 1; i < a.size(); ++i) {
        termina_aqui = std::max(a[i], termina_aqui + a[i]);
        melhor = std::max(melhor, termina_aqui);
    }
    return melhor;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, l = 0, r = 0;
    std::cin >> n >> l >> r;
    std::vector<long long> variacao(n);
    for (auto& x : variacao) std::cin >> x;
    const std::span<const long long> janela =
        std::span<const long long>(variacao).subspan(static_cast<std::size_t>(l - 1), static_cast<std::size_t>(r - l + 1));
    std::println("{}", melhor_trecho(janela));
}
