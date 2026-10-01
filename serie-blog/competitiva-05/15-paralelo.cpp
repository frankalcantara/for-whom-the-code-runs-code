// Políticas de execução: trabalho leve limitado pela memória contra trabalho pesado por elemento.
#include <algorithm>
#include <execution>
#include <numeric>
#include <print>
#include <thread>
#include <vector>
#include "medicao.hpp"

// Trabalho pesado e independente por elemento: 64 rodadas de mistura de bits, não lineares.
inline unsigned pesado(unsigned x) {
    for (int k = 0; k < 64; ++k) { x ^= x >> 13; x *= 0x5bd1'e995u; x ^= x >> 15; }
    return x;
}

int main() {
    std::println("threads de hardware: {}", std::thread::hardware_concurrency());
    constexpr int N = 20'000'000;
    std::vector<unsigned> v(N);
    std::iota(v.begin(), v.end(), 0u);

    auto leve = [&](auto politica) {
        return [&, politica] {
            std::for_each(politica, v.begin(), v.end(), [](unsigned& x) { x = x * 3u + 1u; });
            return static_cast<long long>(v[12345]);
        };
    };
    auto pesado_em = [&](auto politica) {
        return [&, politica] {
            std::for_each(politica, v.begin(), v.end(), [](unsigned& x) { x = pesado(x); });
            return static_cast<long long>(v[12345]);
        };
    };
    long long obs = 0;
    const double l_seq = medir(leve(std::execution::seq), obs).mediana_ms;
    const double l_par = medir(leve(std::execution::par_unseq), obs).mediana_ms;
    const double p_seq = medir(pesado_em(std::execution::seq), obs).mediana_ms;
    const double p_par = medir(pesado_em(std::execution::par_unseq), obs).mediana_ms;
    std::println("2 * 10^7 elementos, mediana de 5 rodadas");
    std::println("leve,   seq       : {:9.3f} ms", l_seq);
    std::println("leve,   par_unseq : {:9.3f} ms", l_par);
    std::println("pesado, seq       : {:9.3f} ms", p_seq);
    std::println("pesado, par_unseq : {:9.3f} ms", p_par);
    std::println("aceleração leve = {:.1f}, aceleração pesado = {:.1f}", l_seq / l_par, p_seq / p_par);

    // A mesma transformação, sequencial e paralela, produz o mesmo vetor.
    std::vector<unsigned> a(1'000'000), b(1'000'000);
    std::iota(a.begin(), a.end(), 7u);
    std::iota(b.begin(), b.end(), 7u);
    std::for_each(std::execution::seq, a.begin(), a.end(), [](unsigned& x) { x = pesado(x); });
    std::for_each(std::execution::par_unseq, b.begin(), b.end(), [](unsigned& x) { x = pesado(x); });
    std::println("resultados iguais: {}", a == b);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
