// A configuração do faroleiro: std::expected distingue quatro falhas, std::optional não.
#include <expected>
#include <filesystem>
#include <fstream>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <vector>

enum class Falha { arquivo_ausente, nao_inteiro, fora_da_faixa, dados_extras };

std::string_view descrever(Falha f) {
    switch (f) {
        case Falha::arquivo_ausente: return "o arquivo não existe";
        case Falha::nao_inteiro:     return "o brilho não é um inteiro";
        case Falha::fora_da_faixa:   return "o brilho está fora de [0, 100]";
        case Falha::dados_extras:    return "há dados depois do valor";
    }
    return "falha desconhecida";
}

std::expected<int, Falha> ler_brilho(const std::filesystem::path& caminho) {
    std::ifstream in(caminho);
    if (!in) return std::unexpected(Falha::arquivo_ausente);
    int brilho = 0;
    if (!(in >> brilho)) return std::unexpected(Falha::nao_inteiro);
    if (brilho < 0 || brilho > 100) return std::unexpected(Falha::fora_da_faixa);
    std::string sobra;
    if (in >> sobra) return std::unexpected(Falha::dados_extras);
    return brilho;
}

// A versão com optional responde se há um brilho válido, mas perde o motivo da falha.
std::optional<int> ler_brilho_opcional(const std::filesystem::path& caminho) {
    const auto r = ler_brilho(caminho);
    return r ? std::optional<int>{*r} : std::nullopt;
}

int main() {
    namespace fs = std::filesystem;
    const fs::path pasta = fs::temp_directory_path() / "competitiva07-farol";
    fs::create_directories(pasta);
    const std::vector<std::pair<std::string, std::string>> casos{
        {"valido", "72\n"}, {"texto", "brilhante\n"}, {"alto", "130\n"}, {"extra", "40 60\n"},
    };
    for (const auto& [nome, texto] : casos) {
        const fs::path p = pasta / (nome + ".cfg");
        std::ofstream(p) << texto;
        const auto r = ler_brilho(p);
        if (r) std::println("{}: brilho={}", nome, *r);
        else   std::println("{}: ERRO: {}", nome, descrever(r.error()));
        std::println("{}: com optional, tem valor = {}", nome, ler_brilho_opcional(p).has_value());
        fs::remove(p);
    }
    const auto r = ler_brilho(pasta / "ausente.cfg");
    std::println("ausente: ERRO: {}", descrever(r.error()));
}
