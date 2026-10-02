// T05.6, as inscrições gêmeas: igualdade de trechos por hash de prefixo módulo 2^61 - 1 com base
// sorteada, em O(1) por pergunta.
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "hash61.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    int q = 0;
    std::cin >> s >> q;
    const int n = static_cast<int>(s.size());
    std::mt19937_64 gerador(std::random_device{}());
    const std::uint64_t base = std::uniform_int_distribution<std::uint64_t>(257, hash61::MOD - 2)(gerador);
    std::vector<std::uint64_t> ph(n + 1, 0), pw(n + 1, 1);
    for (int i = 0; i < n; ++i) {
        ph[i + 1] = hash61::add(hash61::mul(ph[i], base), static_cast<unsigned char>(s[i]));
        pw[i + 1] = hash61::mul(pw[i], base);
    }
    auto trecho = [&](int l, int r) { return hash61::add(ph[r], hash61::MOD - hash61::mul(ph[l], pw[r - l])); };
    std::string saida;
    for (int t = 0; t < q; ++t) {
        int a = 0, b = 0, len = 0;
        std::cin >> a >> b >> len;
        --a; --b;                                       // trechos [a, a + len) e [b, b + len)
        saida += trecho(a, a + len) == trecho(b, b + len) ? "YES\n" : "NO\n";
    }
    std::cout << saida;
}
