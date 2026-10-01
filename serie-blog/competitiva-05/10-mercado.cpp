// O mercado dos ecos desiguais: mediana inferior das frequências com nth_element.
#include <algorithm>
#include <iostream>
#include <print>
#include <unordered_map>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::unordered_map<long long, int> freq;
    freq.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        long long codigo = 0;
        std::cin >> codigo;
        ++freq[codigo];
    }
    std::vector<int> contagens;
    contagens.reserve(freq.size());
    for (const auto& [codigo, f] : freq) contagens.push_back(f);
    const std::size_t k = (contagens.size() - 1) / 2;
    std::ranges::nth_element(contagens, contagens.begin() + static_cast<std::ptrdiff_t>(k));
    std::println("{}", contagens[k]);
}
