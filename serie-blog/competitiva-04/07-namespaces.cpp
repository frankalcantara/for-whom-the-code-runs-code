// Um componente reutilizável em seu próprio namespace e apelidos de namespace.
#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

namespace dsu {
    std::vector<int> pai;
    void iniciar(int n) { pai.resize(n); std::iota(pai.begin(), pai.end(), 0); }
    int find(int x) {
        while (pai[x] != x) x = pai[x] = pai[pai[x]];   // compressão pela metade
        return x;
    }
    void unir(int a, int b) { pai[find(a)] = find(b); }
}

namespace rng = std::ranges;

int main() {
    dsu::iniciar(6);
    dsu::unir(0, 1);
    dsu::unir(2, 3);
    dsu::unir(1, 3);
    std::println("find(0) == find(2): {}, find(4) == find(5): {}", dsu::find(0) == dsu::find(2), dsu::find(4) == dsu::find(5));
    const std::vector<int> v{5, 8, 13};
    const auto it = rng::find(v, 8);                       // std::ranges::find, sem ambiguidade com dsu::find
    std::println("rng::find encontrou 8 na posição {}", it - v.begin());
}
