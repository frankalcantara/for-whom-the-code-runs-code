// O erro do copista em O(n): uma tabela de contagem no lugar da ordenação.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<int> vezes(n + 1, 0);                  // vezes[r]: quantas vezes o rótulo r apareceu
    for (int i = 0; i < n; ++i) {
        int r = 0;
        std::cin >> r;
        ++vezes[r];
    }
    int repetido = 0, ausente = 0;
    for (int r = 1; r <= n; ++r) {
        if (vezes[r] == 2) repetido = r;
        if (vezes[r] == 0) ausente = r;
    }
    std::println("{} {}", repetido, ausente);
}
