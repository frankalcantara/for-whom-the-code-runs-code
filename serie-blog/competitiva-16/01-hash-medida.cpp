// Hash de prefixo de 10^7 letras: módulo 2^64 pelo transbordamento, módulo 2^61 - 1 com o produto de
// 128 bits e módulo 2^61 - 1 com o produto montado por partes de 32 bits, sem extensões.
#include <cstdint>
#include <print>
#include <random>
#include <string>
#include <vector>
#include "hash61.hpp"
#include "medicao.hpp"

// Produto módulo 2^61 - 1 só com operações de 64 bits: a = a1 * 2^31 + a0, b = b1 * 2^31 + b0.
std::uint64_t mul_partes(std::uint64_t a, std::uint64_t b) {
    constexpr std::uint64_t M = hash61::MOD, M31 = (std::uint64_t{1} << 31) - 1, M30 = (std::uint64_t{1} << 30) - 1;
    const std::uint64_t a1 = a >> 31, a0 = a & M31, b1 = b >> 31, b0 = b & M31;
    const std::uint64_t meio = a0 * b1 + a1 * b0;              // < 2^62
    const std::uint64_t alto = a1 * b1;                        // < 2^60, vale alto * 2^62 = 2 * alto
    std::uint64_t r = 2 * alto + (meio >> 30) + ((meio & M30) << 31) + a0 * b0;
    r = (r & M) + (r >> 61);
    if (r >= M) r -= M;
    return r;
}

int main() {
    std::mt19937_64 gerador(20261016);
    const int n = 10'000'000;
    std::string s(n, 'a');
    for (auto& c : s) c = static_cast<char>('a' + gerador() % 26);
    const std::uint64_t base64 = gerador() | 1;
    const std::uint64_t base61 = std::uniform_int_distribution<std::uint64_t>(257, hash61::MOD - 2)(gerador);
    std::vector<std::uint64_t> ph(n + 1), pw(n + 1);
    long long obs = 0;
    auto nada = [] {};
    auto constroi_64 = [&] {
        ph[0] = 0; pw[0] = 1;
        for (int i = 0; i < n; ++i) { ph[i + 1] = ph[i] * base64 + static_cast<unsigned char>(s[i]); pw[i + 1] = pw[i] * base64; }
        return ph[n];
    };
    auto constroi_128 = [&] {
        ph[0] = 0; pw[0] = 1;
        for (int i = 0; i < n; ++i) {
            ph[i + 1] = hash61::add(hash61::mul(ph[i], base61), static_cast<unsigned char>(s[i]));
            pw[i + 1] = hash61::mul(pw[i], base61);
        }
        return ph[n];
    };
    auto constroi_partes = [&] {
        ph[0] = 0; pw[0] = 1;
        for (int i = 0; i < n; ++i) {
            ph[i + 1] = hash61::add(mul_partes(ph[i], base61), static_cast<unsigned char>(s[i]));
            pw[i + 1] = mul_partes(pw[i], base61);
        }
        return ph[n];
    };
    std::uint64_t h128 = 0, hpartes = 0;
    const auto m1 = medir_com_preparo(nada, constroi_64, obs);
    const auto m2 = medir_com_preparo(nada, [&] { h128 = constroi_128(); return h128; }, obs);
    const auto m3 = medir_com_preparo(nada, [&] { hpartes = constroi_partes(); return hpartes; }, obs);
    // conferência do produto por partes contra o de 128 bits em valores aleatórios
    bool confere = h128 == hpartes;
    std::uniform_int_distribution<std::uint64_t> v(0, hash61::MOD - 1);
    for (int t = 0; t < 1'000'000 && confere; ++t) { const auto a = v(gerador), b = v(gerador); confere = hash61::mul(a, b) == mul_partes(a, b); }
    std::println("construção de 10^7 prefixos: módulo 2^64 {:.2f} ms, 2^61 - 1 com 128 bits {:.2f} ms, 2^61 - 1 por partes {:.2f} ms",
                 m1.mediana_ms, m2.mediana_ms, m3.mediana_ms);
    std::println("produto por partes igual ao de 128 bits: {}", confere);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
