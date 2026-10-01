// Protocolo de medição do Artigo 1: aquecimento, rodadas, mediana e mínimo.
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>

struct Medicao {
    double minimo_ms;
    double mediana_ms;
};

// Executa a função uma vez para aquecer e depois Rodadas vezes, cronometrando cada uma.
// A função devolve um valor que o chamador acumula e imprime, o que impede a
// eliminação da chamada como código morto.
template <std::size_t Rodadas = 5, typename Funcao>
Medicao medir(Funcao&& funcao, long long& observado) {
    static_assert(Rodadas % 2 == 1);
    using relogio = std::chrono::steady_clock;
    observado += static_cast<long long>(funcao());
    std::array<double, Rodadas> ms{};
    for (auto& d : ms) {
        const auto t0 = relogio::now();
        observado += static_cast<long long>(funcao());
        const auto t1 = relogio::now();
        d = std::chrono::duration<double, std::milli>(t1 - t0).count();
    }
    std::ranges::sort(ms);
    return {ms.front(), ms[Rodadas / 2]};
}
