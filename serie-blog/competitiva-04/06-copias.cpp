// O custo de copiar em um laço: auto, const auto& e a mesma soma.
#include <print>
#include <string>
#include <vector>
#include "medicao.hpp"

int main() {
    // 200 000 strings de 40 caracteres: grandes demais para a otimização de strings curtas.
    std::vector<std::string> palavras(200'000, std::string(40, 'a'));
    for (std::size_t i = 0; i < palavras.size(); ++i) palavras[i][i % 40] = 'b';

    auto por_copia = [&] {
        std::size_t total = 0;
        for (auto s : palavras) total += s.size() + static_cast<unsigned char>(s[0]);
        return total;
    };
    auto por_referencia = [&] {
        std::size_t total = 0;
        for (const auto& s : palavras) total += s.size() + static_cast<unsigned char>(s[0]);
        return total;
    };
    long long obs = 0;
    const Medicao c = medir(por_copia, obs);
    const Medicao r = medir(por_referencia, obs);
    std::println("200000 strings de 40 caracteres, mediana de 5 rodadas");
    std::println("for (auto s : ...)        : {:8.3f} ms", c.mediana_ms);
    std::println("for (const auto& s : ...) : {:8.3f} ms", r.mediana_ms);
    std::println("razão cópia / referência = {:.1f}", c.mediana_ms / r.mediana_ms);
    std::println("resultados iguais: {}", por_copia() == por_referencia());
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
