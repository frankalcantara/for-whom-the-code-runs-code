// T03.4, as doses do boticário: a gramática a/b validada em ordem, com std::expected.
#include <expected>
#include <iostream>
#include <print>
#include <string>
#include <string_view>

struct Dose {
    long long a, b;
};

std::expected<Dose, std::string_view> analisar(std::string_view linha) {
    std::size_t i = 0;
    const bool negativo = i < linha.size() && linha[i] == '-';
    if (negativo) ++i;
    auto algarismos = [&](long long& saida) {          // lê um ou mais algarismos, no máximo 16
        const std::size_t inicio = i;
        saida = 0;
        while (i < linha.size() && linha[i] >= '0' && linha[i] <= '9') {
            if (i - inicio == 16) return false;        // mais longo que qualquer valor válido
            saida = saida * 10 + (linha[i] - '0');
            ++i;
        }
        return i > inicio;
    };
    long long a = 0, b = 0;
    if (!algarismos(a)) return std::unexpected("bad format");
    if (i == linha.size() || linha[i] != '/') return std::unexpected("bad format");
    ++i;
    if (!algarismos(b)) return std::unexpected("bad format");
    if (i != linha.size()) return std::unexpected("bad format");    // nada depois do divisor
    if (b == 0) return std::unexpected("division by zero");         // só depois de a linha ser válida
    return Dose{negativo ? -a : a, b};
}

int main() {
    int q = 0;
    std::cin >> q;
    std::string linha;
    std::getline(std::cin, linha);                    // descarta o resto da primeira linha
    while (q-- > 0) {
        std::getline(std::cin, linha);
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();
        if (const auto dose = analisar(linha)) std::println("{}/{} = {}", dose->a, dose->b, dose->a / dose->b);
        else                                   std::println("REFUSED {}", dose.error());
    }
}
