// std::print, std::println e std::format.
#include <format>
#include <print>
#include <string>
#include <utility>
#include <vector>

int main() {
    const long long x{1'000'000'000'000LL};
    std::println("{}", x);

    // Campos alinhados à direita com largura fixa.
    std::println("{:>8}  {:>12}  {:>12}", "n", "ordenação", "dispersão");
    std::println("{:>8}  {:>12}  {:>12}", 1000, 420, 1020);
    std::println("{:>8}  {:>12}  {:>12}", 1000000, 802698, 1088176);

    // Precisão, notação científica, hexadecimal e binário.
    std::println("{:.3f} {:.2e} {:#x} {:#b}", 3.14159265, 12345.678, 255, 5);

    // std::format devolve uma string para ser guardada ou combinada.
    const std::string linha = std::format("n={} tempo={}us", 100000, 71417);
    std::println("{} ({} caracteres)", linha, linha.size());

    // Vinculação estruturada ao percorrer pares.
    const std::vector<std::pair<std::string, int>> placar{{"Ana", 3}, {"Bia", 5}};
    for (const auto& [nome, pontos] : placar) std::println("{} {}", nome, pontos);
}
