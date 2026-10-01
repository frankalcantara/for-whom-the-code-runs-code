// O arquivo meteorológico: std::expected carrega as estatísticas ou o motivo da recusa.
// Com um argumento, processa o arquivo indicado. Sem argumentos, executa os casos de teste.
#include <algorithm>
#include <climits>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <string>
#include <vector>

struct Estatisticas {
    long long soma, minimo, maximo;
};

std::expected<Estatisticas, std::string> ler_arquivo(const std::filesystem::path& caminho) {
    std::ifstream in(caminho);
    if (!in) return std::unexpected("não foi possível abrir o arquivo");
    long long n = 0;
    if (!(in >> n)) return std::unexpected("a contagem está ausente ou não é um inteiro");
    if (n <= 0) return std::unexpected("a contagem precisa ser positiva");
    Estatisticas e{0, LLONG_MAX, LLONG_MIN};
    for (long long i = 0; i < n; ++i) {
        long long x = 0;
        if (!(in >> x)) return std::unexpected(std::format("a medida {} está ausente ou malformada", i + 1));
        e.soma += x;
        e.minimo = std::min(e.minimo, x);
        e.maximo = std::max(e.maximo, x);
    }
    std::string sobra;
    if (in >> sobra) return std::unexpected("há dados depois da última medida");
    return e;
}

void relatar(const std::filesystem::path& caminho) {
    const auto r = ler_arquivo(caminho);
    if (r) std::println("{} {} {}", r->soma, r->minimo, r->maximo);
    else   std::println("ERRO: {}", r.error());
}

int main(int argc, char** argv) {
    if (argc == 2) { relatar(argv[1]); return 0; }
    namespace fs = std::filesystem;
    const fs::path pasta = fs::temp_directory_path() / "competitiva07-meteorologia";
    fs::create_directories(pasta);
    const std::vector<std::pair<std::string, std::string>> casos{
        {"valido", "5\n12 -3 8 8 1\n"},
        {"contagem_ausente", ""},
        {"contagem_zero", "0\n"},
        {"medida_malformada", "3\n1 x 3\n"},
        {"medida_faltando", "4\n1 2 3\n"},
        {"dados_extras", "2\n5 6 7\n"},
    };
    for (const auto& [nome, texto] : casos) {
        const fs::path p = pasta / (nome + ".txt");
        std::ofstream(p) << texto;
        std::print("{}: ", nome);
        relatar(p);
        fs::remove(p);
    }
    std::print("inexistente: ");
    relatar(pasta / "nao_existe.txt");
}
