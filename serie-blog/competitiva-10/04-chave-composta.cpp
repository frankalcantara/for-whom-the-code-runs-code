// Ordem composta, pontos decrescentes e id crescente, escrita de quatro formas.
#include <algorithm>
#include <print>
#include <random>
#include <tuple>
#include <vector>
#include "medicao.hpp"

struct Competidor {
    int pontos;
    int id;
};

// Chave de 40 bits: pontos invertidos nos 20 bits altos, id nos 20 bits baixos.
long long chave(const Competidor& c) {
    const long long invertido = ((1LL << 20) - 1) - c.pontos;   // decrescente por inversão
    return (invertido << 20) | c.id;
}

int main() {
    const int n = 5'000'000;
    std::mt19937 gerador(12345);
    std::uniform_int_distribution<int> dist(0, (1 << 20) - 1);  // pontos e id menores que 2^20
    std::vector<Competidor> base(n);
    for (auto& c : base) c = {dist(gerador), dist(gerador)};

    long long obs = 0;
    std::vector<Competidor> v;
    std::vector<long long> chaves(n);
    auto copia = [&] { v = base; };

    const auto a = medir_com_preparo(copia, [&] {
        std::ranges::sort(v, [](const Competidor& x, const Competidor& y) {
            if (x.pontos != y.pontos) return x.pontos > y.pontos;
            return x.id < y.id;
        });
        return v[0].id;
    }, obs);
    std::vector<Competidor> referencia = v;

    const auto b = medir_com_preparo(copia, [&] {
        std::ranges::sort(v, {}, [](const Competidor& c) { return std::tuple{-c.pontos, c.id}; });
        return v[0].id;
    }, obs);
    const bool ok_b = std::ranges::equal(v, referencia, {}, &Competidor::id, &Competidor::id);

    const auto c = medir_com_preparo(copia, [&] {
        std::ranges::sort(v, {}, chave);
        return v[0].id;
    }, obs);
    const bool ok_c = std::ranges::equal(v, referencia, {}, &Competidor::id, &Competidor::id);

    const auto c2 = medir_com_preparo(copia, [&] {
        std::ranges::sort(v, {}, [](const Competidor& x) { return chave(x); });   // a mesma chave, por lambda
        return v[0].id;
    }, obs);
    const bool ok_c2 = std::ranges::equal(v, referencia, {}, &Competidor::id, &Competidor::id);

    const auto d = medir_com_preparo([] {}, [&] {
        for (int i = 0; i < n; ++i) chaves[i] = chave(base[i]);  // a chave carrega pontos e id
        std::ranges::sort(chaves);
        return chaves[0];
    }, obs);
    bool ok_d = true;
    for (int i = 0; i < n; ++i) ok_d = ok_d && static_cast<int>(chaves[i] & ((1 << 20) - 1)) == referencia[i].id;

    std::println("5 * 10^6 competidores, pontos decrescentes e id crescente");
    std::println("  comparação com dois testes:    {:.1f} ms", a.mediana_ms);
    std::println("  projeção para tupla:           {:.1f} ms, mesma ordem: {}", b.mediana_ms, ok_b);
    std::println("  chave inteira, nome da função: {:.1f} ms, mesma ordem: {}", c.mediana_ms, ok_c);
    std::println("  chave inteira, lambda:         {:.1f} ms, mesma ordem: {}", c2.mediana_ms, ok_c2);
    std::println("  vetor de chaves inteiras:      {:.1f} ms, mesma ordem: {}", d.mediana_ms, ok_d);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
