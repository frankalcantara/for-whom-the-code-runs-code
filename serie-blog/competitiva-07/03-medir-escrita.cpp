// Seis formas de escrever os inteiros de 1 a n, uma por linha, medidas em processos separados.
#include <algorithm>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <print>
#include <string>
#include <vector>
#include "processos.hpp"
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

namespace fs = std::filesystem;
using relogio = std::chrono::steady_clock;

// Escritor com buffer: converte com std::to_chars e descarrega com fwrite em blocos de 1 MiB.
class EscritorRapido {
    static constexpr std::size_t capacidade = 1u << 20;
    char buffer[capacidade];
    std::size_t pos = 0;
public:
    void descarregar() { std::fwrite(buffer, 1, pos, stdout); pos = 0; }
    void escrever(long long v) {
        if (capacidade - pos < 24) descarregar();
        pos = static_cast<std::size_t>(std::to_chars(buffer + pos, buffer + capacidade, v).ptr - buffer);
        buffer[pos++] = '\n';
    }
    ~EscritorRapido() { descarregar(); }
};

int filho(const std::string& modo, long long n) {
    const auto t0 = relogio::now();
    if (modo == "endl") {
        for (long long i = 1; i <= n; ++i) std::cout << i << std::endl;
    } else if (modo == "barra_n") {
        std::ios_base::sync_with_stdio(false);
        for (long long i = 1; i <= n; ++i) std::cout << i << '\n';
        std::cout.flush();
    } else if (modo == "printf") {
        for (long long i = 1; i <= n; ++i) std::printf("%lld\n", i);
        std::fflush(stdout);
    } else if (modo == "print") {
        for (long long i = 1; i <= n; ++i) std::println("{}", i);
        std::fflush(stdout);
    } else if (modo == "buffer" || modo == "buffer_binario") {
#ifdef _WIN32
        if (modo == "buffer_binario") _setmode(_fileno(stdout), _O_BINARY);   // sem tradução de \n para \r\n
#endif
        static EscritorRapido saida;
        for (long long i = 1; i <= n; ++i) saida.escrever(i);
        saida.descarregar();
        std::fflush(stdout);
    } else {
        return 2;
    }
    const auto t1 = relogio::now();
    std::println(stderr, "{}", std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count());
    return 0;
}

// Confere que o arquivo contém exatamente 1, 2, ..., n, um por linha, aceitando \r\n.
bool conferir(const std::string& texto, long long n, std::size_t& quebras_crlf) {
    long long esperado = 1, v = 0;
    bool em_numero = false;
    quebras_crlf = 0;
    for (std::size_t i = 0; i < texto.size(); ++i) {
        const char c = texto[i];
        if (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); em_numero = true; }
        else if (c == '\r') { ++quebras_crlf; }
        else if (c == '\n') {
            if (!em_numero || v != esperado) return false;
            ++esperado; v = 0; em_numero = false;
        } else return false;
    }
    return esperado == n + 1;
}

int main(int argc, char** argv) {
    if (argc >= 3) return filho(argv[1], std::stoll(argv[2]));

    const fs::path programa = fs::absolute(argv[0]);
    const fs::path pasta = fs::temp_directory_path() / "competitiva07-escrita";
    fs::create_directories(pasta);
    const fs::path saida = pasta / "saida.txt", erros = pasta / "tempo.txt";
    constexpr long long n = 1'000'000;
    std::vector<std::string> modos{"endl", "barra_n", "printf", "print", "buffer"};
#ifdef _WIN32
    modos.push_back("buffer_binario");
#endif
    std::println("{:<14} | {:>12} | {:>13} | {:>12}", "modo", "mediana (ms)", "bytes gerados", "quebras \\r\\n");
    bool tudo_certo = true;
    for (const auto& modo : modos) {
        std::vector<double> ms;
        std::size_t bytes = 0, crlf = 0;
        for (int rodada = 0; rodada < 6; ++rodada) {
            if (processo::executar(programa, modo + " " + std::to_string(n), {}, saida, erros) != 0) tudo_certo = false;
            const std::string texto = processo::conteudo(saida);
            if (!conferir(texto, n, crlf)) tudo_certo = false;
            bytes = texto.size();
            if (rodada > 0) ms.push_back(std::stoll(processo::conteudo(erros)) / 1000.0);
        }
        std::ranges::sort(ms);
        std::println("{:<14} | {:>12.3f} | {:>13} | {:>12}", modo, ms[ms.size() / 2], bytes, crlf);
    }
    std::println("saídas conferidas em todos os modos: {}", tudo_certo);
}
