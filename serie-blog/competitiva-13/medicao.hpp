// Protocolo de medição do Artigo 1: aquecimento, rodadas, mediana e mínimo.
// Para ordenação, cada rodada precisa de uma cópia nova dos dados: o preparo
// roda fora do cronômetro, e só a função medida entra no tempo.
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>

struct Medicao {
    double minimo_ms;
    double mediana_ms;
};

template <std::size_t Rodadas = 5, typename Preparo, typename Funcao>
Medicao medir_com_preparo(Preparo&& preparo, Funcao&& funcao, long long& observado) {
    static_assert(Rodadas % 2 == 1);
    using relogio = std::chrono::steady_clock;
    preparo();
    observado += static_cast<long long>(funcao());       // aquecimento
    std::array<double, Rodadas> ms{};
    for (auto& d : ms) {
        preparo();
        const auto t0 = relogio::now();
        observado += static_cast<long long>(funcao());
        const auto t1 = relogio::now();
        d = std::chrono::duration<double, std::milli>(t1 - t0).count();
    }
    std::ranges::sort(ms);
    return {ms.front(), ms[Rodadas / 2]};
}
