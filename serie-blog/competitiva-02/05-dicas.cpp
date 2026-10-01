// Dicas ao compilador: [[unlikely]], std::unreachable e [[assume]].
#include <cstdint>
#include <print>
#include <random>
#include <utility>
#include <vector>
#include "medicao.hpp"

enum class Estado : std::uint8_t { A, B, C, D };

struct Instrucao {
    Estado estado;
    int valor;
};

int tratar_padrao(Estado e, int v) {
    switch (e) {
    case Estado::A: return v + 1;
    case Estado::B: return v - 1;
    case Estado::C: return v * 2;
    case Estado::D: return v / 2;
    default: return 0;  // caminho defensivo, nunca alcançado com dados válidos
    }
}

long long processar_padrao(const std::vector<Instrucao>& fluxo) {
    long long total = 0;
    for (const auto& ins : fluxo) {
        if (ins.valor == 0) { total += 1; continue; }
        total += tratar_padrao(ins.estado, ins.valor);
    }
    return total;
}

int tratar_com_dica(Estado e, int v) {
    switch (e) {
    case Estado::A: return v + 1;
    case Estado::B: return v - 1;
    case Estado::C: return v * 2;
    case Estado::D: return v / 2;
    }
    std::unreachable();  // promessa: todos os estados foram tratados acima
}

long long processar_com_dica(const std::vector<Instrucao>& fluxo) {
    [[assume(fluxo.size() % 4 == 0)]];  // promessa verdadeira para o fluxo gerado
    long long total = 0;
    for (const auto& ins : fluxo) {
        if (ins.valor == 0) [[unlikely]] { total += 1; continue; }
        total += tratar_com_dica(ins.estado, ins.valor);
    }
    return total;
}

int main() {
    const std::size_t n = 40'000'000;  // múltiplo de 4, como a promessa exige
    std::mt19937 gerador(12345);
    std::uniform_int_distribution<int> valores(0, 1'000'000);
    std::uniform_int_distribution<int> estados(0, 3);
    std::vector<Instrucao> fluxo(n);
    for (auto& ins : fluxo) {
        ins.valor = valores(gerador) < 10 ? 0 : valores(gerador);  // zero é raríssimo
        ins.estado = static_cast<Estado>(estados(gerador));
    }
    long long obs = 0;
    const Medicao padrao = medir([&] { return processar_padrao(fluxo); }, obs);
    const Medicao dica = medir([&] { return processar_com_dica(fluxo); }, obs);
    std::println("n = {}, 1 aquecimento e 5 rodadas, mediana em ms", n);
    std::println("sem dicas: {:7.1f}", padrao.mediana_ms);
    std::println("com dicas: {:7.1f}", dica.mediana_ms);
    std::println("resultados iguais: {}", processar_padrao(fluxo) == processar_com_dica(fluxo));
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
