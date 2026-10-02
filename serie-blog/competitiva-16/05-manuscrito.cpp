// Exercício 5.9, o manuscrito do plagiador: o maior trecho que aparece duas vezes, por busca binária
// no comprimento e hash de prefixo com base sorteada, confirmando cada coincidência caractere a caractere.
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <string_view>
#include <vector>
#include "hash61.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::string s;
    std::cin >> n >> s;
    std::mt19937_64 gerador(std::random_device{}());
    const std::uint64_t base = std::uniform_int_distribution<std::uint64_t>(257, hash61::MOD - 2)(gerador);
    std::vector<std::uint64_t> ph(n + 1, 0), pw(n + 1, 1);
    for (int i = 0; i < n; ++i) {
        ph[i + 1] = hash61::add(hash61::mul(ph[i], base), static_cast<unsigned char>(s[i]));
        pw[i + 1] = hash61::mul(pw[i], base);
    }
    auto trecho = [&](int l, int r) {                         // hash de s[l, r)
        return hash61::add(ph[r], hash61::MOD - hash61::mul(ph[l], pw[r - l]));
    };
    const std::string_view sv(s);
    std::vector<std::pair<std::uint64_t, int>> janelas;        // (hash, início)
    auto repete = [&](int L) {
        if (L == 0) return true;
        janelas.clear();
        for (int i = 0; i + L <= n; ++i) janelas.emplace_back(trecho(i, i + L), i);
        std::ranges::sort(janelas);                            // hashes iguais ficam vizinhos
        for (std::size_t j = 1; j < janelas.size(); ++j)
            if (janelas[j].first == janelas[j - 1].first &&
                sv.substr(janelas[j].second, L) == sv.substr(janelas[j - 1].second, L))
                return true;                                   // confirmado caractere a caractere
        return false;
    };
    int lo = 0, hi = n - 1;                                    // o trecho inteiro não tem onde se repetir
    while (lo < hi) {
        const int meio = lo + (hi - lo + 1) / 2;
        if (repete(meio)) lo = meio;
        else hi = meio - 1;
    }
    std::println("{}", lo);
}
