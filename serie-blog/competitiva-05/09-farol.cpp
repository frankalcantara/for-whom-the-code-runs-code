// A janela do farol: a coluna de maior soma, com desempate pelo menor índice.
#include <algorithm>
#include <iostream>
#include <iterator>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int r = 0, c = 0;
    std::cin >> r >> c;
    std::vector<long long> soma_coluna(c, 0);   // até 2000 * 10^9: exige 64 bits
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j) {
            long long luz = 0;
            std::cin >> luz;
            soma_coluna[j] += luz;
        }
    const auto melhor = std::ranges::max_element(soma_coluna);   // primeiro máximo
    std::println("{} {}", std::distance(soma_coluna.begin(), melhor) + 1, *melhor);
}
