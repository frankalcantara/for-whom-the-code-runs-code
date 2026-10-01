// Protocolo de medição: aquecimento, repetições, mediana, mínimo e validação.
// Compilar: cl /std:c++latest /O2 /EHsc medir.cpp
//       ou: clang++ -std=c++23 -O2 medir.cpp -o medir
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <print>
#include <vector>

struct Medicao {
    long long minimo_us;
    long long mediana_us;
};

// O parâmetro Rodadas é conhecido em tempo de compilação, o que permite guardar
// as durações em um std::array, sem alocação dentro da região medida.
template <std::size_t Rodadas = 7, typename Funcao>
Medicao medir(Funcao&& funcao) {
    static_assert(Rodadas % 2 == 1, "um número ímpar de rodadas tem mediana única");
    using relogio = std::chrono::steady_clock;

    funcao();  // aquecimento: executado e descartado

    std::array<long long, Rodadas> duracoes{};
    for (auto& d : duracoes) {
        const auto t0 = relogio::now();
        funcao();
        const auto t1 = relogio::now();
        d = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    }
    std::ranges::sort(duracoes);
    return {duracoes.front(), duracoes[Rodadas / 2]};
}

int main() {
    std::vector<std::int32_t> dados(10'000'000);
    std::iota(dados.begin(), dados.end(), 0);  // 0, 1, 2, ..., n - 1

    // O resultado é guardado fora da função medida e impresso no fim,
    // o que impede o compilador de eliminar a soma como código morto.
    long long resultado = 0;
    const Medicao m = medir([&] {
        resultado = std::accumulate(dados.begin(), dados.end(), 0LL);
    });

    const long long n = static_cast<long long>(dados.size());
    const long long esperado = n * (n - 1) / 2;
    std::println("soma = {} (esperado {}): {}", resultado, esperado,
                 resultado == esperado ? "correta" : "ERRADA");
    std::println("mínimo = {} us, mediana = {} us", m.minimo_us, m.mediana_us);
}
