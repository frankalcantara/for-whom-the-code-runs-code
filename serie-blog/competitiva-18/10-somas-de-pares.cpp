// As k menores somas a[i] + b[j]: um heap de mínimo como fronteira da grade de pares, expandindo
// cada par extraído para a direita e para baixo.
#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, m = 0, k = 0;
    std::cin >> n >> m >> k;
    std::vector<long long> a(n), b(m);
    for (auto& x : a) std::cin >> x;
    for (auto& x : b) std::cin >> x;
    std::ranges::sort(a);
    std::ranges::sort(b);
    using Item = std::tuple<long long, int, int>;       // (soma, i, j)
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> fronteira;
    std::unordered_set<std::uint64_t> visto;            // pares já postos na fronteira
    visto.reserve(static_cast<std::size_t>(2 * k + 2));
    auto chave = [m](int i, int j) { return static_cast<std::uint64_t>(i) * static_cast<std::uint64_t>(m) + static_cast<std::uint64_t>(j); };
    fronteira.emplace(a[0] + b[0], 0, 0);
    visto.insert(chave(0, 0));
    std::string saida;
    for (int t = 0; t < k; ++t) {
        const auto [s, i, j] = fronteira.top();
        fronteira.pop();
        saida += std::to_string(s);
        saida += (t + 1 < k ? ' ' : '\n');
        if (i + 1 < n && visto.insert(chave(i + 1, j)).second) fronteira.emplace(a[i + 1] + b[j], i + 1, j);
        if (j + 1 < m && visto.insert(chave(i, j + 1)).second) fronteira.emplace(a[i] + b[j + 1], i, j + 1);
    }
    std::cout << saida;
}
