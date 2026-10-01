// O livro do mosaico: matriz guardada em um único vetor, em ordem de linhas.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int r = 0, c = 0;
    std::cin >> r >> c;
    std::vector<int> buffer(static_cast<std::size_t>(r) * c);
    for (auto& x : buffer) std::cin >> x;       // a leitura já segue a ordem de linhas
    for (int i = 0; i < r; ++i) {
        long long soma = 0;
        const std::size_t inicio = static_cast<std::size_t>(i) * c;
        for (int j = 0; j < c; ++j) soma += buffer[inicio + j];   // índice i * c + j
        std::println("{}", soma);
    }
}
