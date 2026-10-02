// A maior queda: o maior p[i] - p[j] com i < j, mantendo o maior preço visto antes de j.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> p(n);
    for (auto& x : p) std::cin >> x;
    long long melhor = 0, maior_antes = p[0];
    for (int j = 1; j < n; ++j) {
        melhor = std::max(melhor, maior_antes - p[j]);   // primeiro a candidata, com o máximo anterior a j
        maior_antes = std::max(maior_antes, p[j]);       // só depois o preço de j entra no máximo
    }
    std::println("{}", melhor);
}
