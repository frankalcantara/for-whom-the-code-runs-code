// O custo de O(n^2) com o bubble sort, comparado com o std::ranges::sort.
#include <algorithm>
#include <print>
#include <random>
#include <utility>
#include <vector>
#include "medicao.hpp"

void bolha(std::vector<int>& v) {
    const int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j + 1 < n - i; ++j)
            if (v[j] > v[j + 1]) std::swap(v[j], v[j + 1]);
}

int main() {
    std::mt19937 gerador(20260930);
    long long obs = 0;
    std::println("{:>7} | {:>14} | {:>12} | {:>12} | {:>10}", "n", "comparações", "bolha (ms)", "sort (ms)", "razão");
    for (int n : {5'000, 10'000, 20'000, 40'000}) {
        std::vector<int> base(n), v;
        std::uniform_int_distribution<int> dist(0, 1'000'000'000);
        for (int& x : base) x = dist(gerador);
        auto copia = [&] { v = base; };
        const auto mb = medir_com_preparo<3>(copia, [&] { bolha(v); return v[n / 2]; }, obs);
        const auto ms = medir_com_preparo<3>(copia, [&] { std::ranges::sort(v); return v[n / 2]; }, obs);
        const long long comps = 1LL * n * (n - 1) / 2;
        std::println("{:>7} | {:>14} | {:>12.1f} | {:>12.3f} | {:>10.0f}", n, comps, mb.mediana_ms, ms.mediana_ms,
                     mb.mediana_ms / ms.mediana_ms);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
