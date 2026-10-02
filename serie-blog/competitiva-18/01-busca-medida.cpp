// Pertinência em 2 * 10^6 chaves estáticas: std::set, vetor ordenado, std::flat_set, e
// std::unordered_set com o hash da biblioteca e com um hash próprio.
#include <algorithm>
#include <cstdint>
#include <print>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>
#if __has_include(<flat_set>)
#include <flat_set>
#endif
#include "medicao.hpp"

struct Embaralha {                                      // finalizador do splitmix64
    std::size_t operator()(std::uint64_t x) const noexcept {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return static_cast<std::size_t>(x ^ (x >> 31));
    }
};

int main() {
    std::mt19937_64 gerador(20261018);
    std::uniform_int_distribution<std::uint64_t> valor(0, 100'000'000);
    std::vector<std::uint64_t> chaves(2'000'000);
    for (auto& x : chaves) x = valor(gerador);
    std::ranges::sort(chaves);
    chaves.erase(std::unique(chaves.begin(), chaves.end()), chaves.end());
    std::vector<std::uint64_t> perguntas(2'000'000);
    std::uniform_int_distribution<std::size_t> qual(0, chaves.size() - 1);
    for (std::size_t i = 0; i < perguntas.size(); ++i)
        perguntas[i] = (i % 2 == 0) ? chaves[qual(gerador)] : valor(gerador);   // metade presente, metade sorteada
    std::shuffle(perguntas.begin(), perguntas.end(), gerador);

    const std::set<std::uint64_t> arvore(chaves.begin(), chaves.end());
    std::unordered_set<std::uint64_t> hash_padrao;
    hash_padrao.reserve(chaves.size());
    hash_padrao.insert(chaves.begin(), chaves.end());
    std::unordered_set<std::uint64_t, Embaralha> hash_proprio;
    hash_proprio.reserve(chaves.size());
    hash_proprio.insert(chaves.begin(), chaves.end());

    long long obs = 0;
    auto nada = [] {};
    auto conta = [&](auto&& pertence) { long long c = 0; for (auto x : perguntas) c += pertence(x) ? 1 : 0; return c; };
    long long r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = -1;
    const auto m1 = medir_com_preparo(nada, [&] { return r1 = conta([&](std::uint64_t x) { return arvore.contains(x); }); }, obs);
    const auto m2 = medir_com_preparo(nada, [&] { return r2 = conta([&](std::uint64_t x) { return std::ranges::binary_search(chaves, x); }); }, obs);
    const auto m3 = medir_com_preparo(nada, [&] { return r3 = conta([&](std::uint64_t x) { return hash_padrao.contains(x); }); }, obs);
    const auto m4 = medir_com_preparo(nada, [&] { return r4 = conta([&](std::uint64_t x) { return hash_proprio.contains(x); }); }, obs);
    std::println("{} chaves distintas, {} perguntas, {} presentes", chaves.size(), perguntas.size(), r1);
    std::println("std::set: {:.1f} ms", m1.mediana_ms);
    std::println("vetor ordenado com binary_search: {:.1f} ms", m2.mediana_ms);
#if defined(__cpp_lib_flat_set)
    const std::flat_set<std::uint64_t> plano(std::sorted_unique, chaves);
    const auto m5 = medir_com_preparo(nada, [&] { return r5 = conta([&](std::uint64_t x) { return plano.contains(x); }); }, obs);
    std::println("std::flat_set: {:.1f} ms", m5.mediana_ms);
#else
    r5 = r1;
    std::println("std::flat_set: indisponível nesta biblioteca");
#endif
    std::println("std::unordered_set, hash da biblioteca: {:.1f} ms", m3.mediana_ms);
    std::println("std::unordered_set, hash próprio: {:.1f} ms", m4.mediana_ms);
    std::println("mesmas respostas: {}", r1 == r2 && r2 == r3 && r3 == r4 && r4 == r5);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
