// Ordenar registros grandes contra ordenar índices: o número de comparações é o mesmo,
// e o número de bytes movidos não é.
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <numeric>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

struct Registro {          // 256 bytes: a chave e uma carga que viaja junto
    int pontos;
    int id;
    char carga[248];
};

struct Pequeno {           // 8 bytes: só a chave e o identificador
    int pontos;
    int id;
};

int main() {
    std::mt19937 gerador(20260930);
    long long obs = 0;

    // 1. Inteiros: ordem crescente, decrescente e por valor absoluto.
    {
        const int n = 1'000'000;
        std::vector<int> base(n), v;
        std::uniform_int_distribution<int> dist(-1'000'000'000, 1'000'000'000);
        for (int& x : base) x = dist(gerador);
        auto copia = [&] { v = base; };
        const auto a = medir_com_preparo(copia, [&] { std::ranges::sort(v); return v[0]; }, obs);
        const auto b = medir_com_preparo(copia, [&] { std::ranges::sort(v, std::greater<>{}); return v[0]; }, obs);
        const auto c = medir_com_preparo(copia, [&] {
            std::ranges::sort(v, {}, [](int x) { return std::abs(x); });
            return v[0];
        }, obs);
        std::println("10^6 int: crescente {:.1f} ms, decrescente {:.1f} ms, por |x| {:.1f} ms", a.mediana_ms, b.mediana_ms,
                     c.mediana_ms);
    }

    // 2. Registros de 256 bytes contra índices.
    const int n = 200'000;
    std::vector<Registro> base(n);
    std::uniform_int_distribution<int> dist(0, 1'000'000);
    for (int i = 0; i < n; ++i) {
        base[i].pontos = dist(gerador);
        base[i].id = i;
        std::fill(std::begin(base[i].carga), std::end(base[i].carga), static_cast<char>(i));
    }
    std::vector<Pequeno> base_p(n);
    for (int i = 0; i < n; ++i) base_p[i] = {base[i].pontos, i};

    std::vector<Registro> v, saida(n);
    std::vector<Pequeno> vp;
    std::vector<int> ordem(n);

    const auto grande = medir_com_preparo([&] { v = base; }, [&] {
        std::ranges::sort(v, {}, &Registro::pontos);
        return v[0].id;
    }, obs);
    const auto pequeno = medir_com_preparo([&] { vp = base_p; }, [&] {
        std::ranges::sort(vp, {}, &Pequeno::pontos);
        return vp[0].id;
    }, obs);
    const auto indices = medir_com_preparo([&] { std::iota(ordem.begin(), ordem.end(), 0); }, [&] {
        std::ranges::sort(ordem, {}, [&](int i) { return base[i].pontos; });
        return ordem[0];
    }, obs);
    const auto indices_e_copia = medir_com_preparo([&] { std::iota(ordem.begin(), ordem.end(), 0); }, [&] {
        std::ranges::sort(ordem, {}, [&](int i) { return base[i].pontos; });
        for (int i = 0; i < n; ++i) saida[i] = base[ordem[i]];      // reorganiza uma vez, no fim
        return saida[0].id;
    }, obs);

    std::vector<Pequeno> pares(n);
    const auto chave_e_indice = medir_com_preparo([] {}, [&] {
        for (int i = 0; i < n; ++i) pares[i] = {base[i].pontos, i};  // chave compacta e índice
        std::ranges::sort(pares, {}, &Pequeno::pontos);
        for (int i = 0; i < n; ++i) saida[i] = base[pares[i].id];
        return saida[0].id;
    }, obs);

    // As versões precisam produzir a mesma sequência de pontos.
    v = base;
    std::ranges::sort(v, {}, &Registro::pontos);
    bool iguais = true;
    for (int i = 0; i < n; ++i) iguais = iguais && v[i].pontos == saida[i].pontos;
    std::println("2 * 10^5 registros, chave pontos, resultados iguais: {}", iguais);
    std::println("  registros de 256 bytes:        {:.1f} ms", grande.mediana_ms);
    std::println("  registros de 8 bytes:          {:.1f} ms", pequeno.mediana_ms);
    std::println("  índices com projeção:          {:.1f} ms", indices.mediana_ms);
    std::println("  índices e uma cópia final:     {:.1f} ms", indices_e_copia.mediana_ms);
    std::println("  chave e índice, cópia final:   {:.1f} ms", chave_e_indice.mediana_ms);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
