// Copiar, mover ou construir no lugar: o custo mora no tipo do elemento.
#include <print>
#include <string>
#include <utility>
#include <vector>
#include "medicao.hpp"

struct Aresta {
    int destino, peso;
};

int main() {
    constexpr int N = 1'000'000;
    const std::string modelo(64, 'x');   // 64 caracteres: sempre no heap

    auto copiando = [&] {
        std::vector<std::string> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) {
            std::string s = modelo;
            v.push_back(s);                // s é um lvalue: cópia
        }
        return v.size() + v.back().size();
    };
    auto movendo = [&] {
        std::vector<std::string> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) {
            std::string s = modelo;
            v.push_back(std::move(s));     // rvalue: o buffer de s é transferido
        }
        return v.size() + v.back().size();
    };
    auto no_lugar = [&] {
        std::vector<std::string> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) v.emplace_back(64, 'x');   // construído no vetor
        return v.size() + v.back().size();
    };
    auto inteiros = [&] {
        std::vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) {
            int x = i;
            v.push_back(std::move(x));     // mover um int é copiar um int
        }
        return v.size() + static_cast<std::size_t>(v.back());
    };
    long long obs = 0;
    const Medicao c = medir(copiando, obs);
    const Medicao m = medir(movendo, obs);
    const Medicao e = medir(no_lugar, obs);
    const Medicao i = medir(inteiros, obs);
    std::println("10^6 strings de 64 caracteres, mediana de 5 rodadas");
    std::println("push_back(s)            : {:8.3f} ms", c.mediana_ms);
    std::println("push_back(std::move(s)) : {:8.3f} ms", m.mediana_ms);
    std::println("emplace_back(64, 'x')   : {:8.3f} ms", e.mediana_ms);
    std::println("10^6 int com std::move  : {:8.3f} ms", i.mediana_ms);

    // O objeto de origem continua válido depois do movimento.
    std::string origem = "caravana";
    std::vector<std::string> destino;
    destino.push_back(std::move(origem));
    origem = "nova";                       // atribuir de novo é permitido
    std::println("destino[0] = {}, origem reatribuída = {}", destino[0], origem);

    std::vector<Aresta> arestas;
    arestas.push_back(Aresta{3, 7});
    arestas.emplace_back(4, 9);            // C++20: agregados aceitam parênteses
    std::println("arestas: ({}, {}) ({}, {})", arestas[0].destino, arestas[0].peso,
                 arestas[1].destino, arestas[1].peso);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
