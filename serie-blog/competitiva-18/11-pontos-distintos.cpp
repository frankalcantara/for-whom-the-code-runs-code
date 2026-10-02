// Os marcos do mapa: quantos pontos distintos há entre n coordenadas, com cada par (x, y) empacotado
// em uma chave de 64 bits e um hash próprio, o finalizador do splitmix64.
#include <cstdint>
#include <iostream>
#include <print>
#include <unordered_set>

struct Embaralha {                                      // misturador de bits do splitmix64
    std::size_t operator()(std::uint64_t x) const noexcept {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return static_cast<std::size_t>(x ^ (x >> 31));
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::unordered_set<std::uint64_t, Embaralha> pontos;
    pontos.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        long long x = 0, y = 0;
        std::cin >> x >> y;
        const auto ux = static_cast<std::uint32_t>(static_cast<std::int32_t>(x));   // |x| <= 10^9 cabe em 32 bits
        const auto uy = static_cast<std::uint32_t>(static_cast<std::int32_t>(y));
        pontos.insert((static_cast<std::uint64_t>(ux) << 32) | uy);                 // a chave é injetiva
    }
    std::println("{}", pontos.size());
}
