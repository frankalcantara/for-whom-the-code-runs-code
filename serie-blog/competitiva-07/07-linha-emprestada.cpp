// A linha emprestada: std::string_view para percorrer, std::string para guardar.
#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>

struct Resultado {
    long long nao_vazias;
    std::string escolhida;       // dona dos caracteres: sobrevive ao buffer do arquivo
};

Resultado inspecionar(const std::filesystem::path& caminho, long long k) {
    std::string dados;
    {
        std::ifstream in(caminho, std::ios::binary);
        std::ostringstream s;
        s << in.rdbuf();
        dados = std::move(s).str();              // o único dono dos bytes do arquivo
    }
    Resultado r{0, "<ausente>"};
    std::string_view resto = dados;              // vista sobre o sufixo ainda não processado
    while (!resto.empty()) {
        const std::size_t quebra = resto.find('\n');
        std::string_view linha = resto.substr(0, quebra);       // O(1): ponteiro e comprimento
        resto = quebra == std::string_view::npos ? std::string_view{} : resto.substr(quebra + 1);
        if (!linha.empty() && linha.back() == '\r') linha.remove_suffix(1);   // aceita \r\n
        if (linha.empty()) continue;
        if (++r.nao_vazias == k) r.escolhida = std::string(linha);        // cópia: precisa de posse
    }
    return r;                                    // dados morre aqui, e as vistas com ele
}

int main(int argc, char** argv) {
    namespace fs = std::filesystem;
    if (argc == 3) {
        const auto r = inspecionar(argv[1], std::stoll(argv[2]));
        std::println("nao_vazias={}\nescolhida={}", r.nao_vazias, r.escolhida);
        return 0;
    }
    const fs::path p = fs::temp_directory_path() / "competitiva07-manuscrito.txt";
    std::ofstream(p, std::ios::binary) << "Norte\r\n\r\nLeste\r\nOeste";   // \r\n e sem quebra final
    for (long long k : {1, 2, 3, 4}) {
        const auto r = inspecionar(p, k);
        std::println("k={}: nao_vazias={} escolhida={}", k, r.nao_vazias, r.escolhida);
    }
    fs::remove(p);
}
