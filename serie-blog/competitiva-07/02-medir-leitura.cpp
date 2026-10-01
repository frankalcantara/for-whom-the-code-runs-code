// Cinco formas de ler os mesmos inteiros da entrada padrão, medidas em processos separados.
// Uso interno: o processo pai gera o arquivo e chama o próprio executável com um modo.
#define _CRT_SECURE_NO_WARNINGS   // o MSVC marca fopen e scanf como inseguras (aviso C4996)
#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>
#include "processos.hpp"

namespace fs = std::filesystem;
using relogio = std::chrono::steady_clock;

// ---------------------------------------------------------------- modos do processo filho
long long ler_cin() {
    long long n = 0, soma = 0;
    std::cin >> n;
    for (long long i = 0; i < n; ++i) { int x = 0; std::cin >> x; soma += x; }
    return soma;
}

long long ler_scanf() {
    long long n = 0, soma = 0;
    if (std::scanf("%lld", &n) != 1) return 0;
    for (long long i = 0; i < n; ++i) { int x = 0; if (std::scanf("%d", &x) == 1) soma += x; }
    return soma;
}

class LeitorRapido {
    static constexpr std::size_t capacidade = 1u << 16;
    char buffer[capacidade]{};
    std::size_t pos = 0, len = 0;
    int proximo() {
        if (pos == len) {
            len = std::fread(buffer, 1, capacidade, stdin);
            pos = 0;
            if (len == 0) return -1;
        }
        return static_cast<unsigned char>(buffer[pos++]);
    }
public:
    long long ler() {
        int c = proximo();
        while (c != -1 && c != '-' && (c < '0' || c > '9')) c = proximo();
        const bool negativo = c == '-';
        if (negativo) c = proximo();
        long long v = 0;
        while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = proximo(); }
        return negativo ? -v : v;
    }
};

long long ler_fread() {
    static LeitorRapido entrada;
    const long long n = entrada.ler();
    long long soma = 0;
    for (long long i = 0; i < n; ++i) soma += entrada.ler();
    return soma;
}

long long ler_from_chars() {                         // lê tudo de uma vez e converte com std::from_chars
    std::vector<char> dados;
    static std::array<char, 1 << 20> bloco;         // 1 MiB: grande demais para a pilha padrão de 1 MiB do Windows
    for (std::size_t lidos; (lidos = std::fread(bloco.data(), 1, bloco.size(), stdin)) > 0;)
        dados.insert(dados.end(), bloco.data(), bloco.data() + lidos);
    const char* p = dados.data();
    const char* fim = p + dados.size();
    auto proximo = [&](long long& v) {
        while (p < fim && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t')) ++p;
        const auto [q, erro] = std::from_chars(p, fim, v);
        p = q;
        return erro == std::errc{};
    };
    long long n = 0, soma = 0, x = 0;
    proximo(n);
    for (long long i = 0; i < n && proximo(x); ++i) soma += x;
    return soma;
}

int filho(const std::string& modo) {
    const auto t0 = relogio::now();
    long long soma = 0;
    if (modo == "cin_padrao") soma = ler_cin();
    else if (modo == "cin_rapido") {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);
        soma = ler_cin();
    }
    else if (modo == "scanf") soma = ler_scanf();
    else if (modo == "fread") soma = ler_fread();
    else if (modo == "from_chars") soma = ler_from_chars();
    else return 2;
    const auto t1 = relogio::now();
    std::println(stderr, "{} {}", soma, std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count());
    return 0;
}

// ---------------------------------------------------------------- processo pai
long long gerar(const fs::path& arquivo, long long n) {
    std::mt19937 gerador(20260930);
    std::uniform_int_distribution<int> dist(-1'000'000, 1'000'000);
    std::FILE* f = std::fopen(arquivo.string().c_str(), "wb");
    std::vector<char> buffer;
    buffer.reserve(1 << 20);
    long long soma = 0;
    char tmp[24];
    auto escrever = [&](long long v, char sep) {
        const auto r = std::to_chars(tmp, tmp + sizeof tmp, v);
        buffer.insert(buffer.end(), tmp, r.ptr);
        buffer.push_back(sep);
        if (buffer.size() > (1 << 20) - 64) { std::fwrite(buffer.data(), 1, buffer.size(), f); buffer.clear(); }
    };
    escrever(n, '\n');
    for (long long i = 0; i < n; ++i) {
        const int x = dist(gerador);
        soma += x;
        escrever(x, (i + 1) % 10 == 0 ? '\n' : ' ');
    }
    std::fwrite(buffer.data(), 1, buffer.size(), f);
    std::fclose(f);
    return soma;
}

int main(int argc, char** argv) {
    if (argc >= 2) return filho(argv[1]);

    const fs::path programa = fs::absolute(argv[0]);
    const fs::path pasta = fs::temp_directory_path() / "competitiva07-leitura";
    fs::create_directories(pasta);
    const fs::path saida = pasta / "saida.txt", erros = pasta / "tempo.txt";

    struct Caso { const char* modo; long long n; };
    const Caso casos[] = {
        {"cin_padrao", 1'000'000}, {"cin_rapido", 1'000'000}, {"scanf", 1'000'000},
        {"fread", 1'000'000}, {"from_chars", 1'000'000},
        {"cin_rapido", 10'000'000}, {"scanf", 10'000'000}, {"fread", 10'000'000}, {"from_chars", 10'000'000},
    };
    std::println("{:<11} | {:>9} | {:>12} | {:>14}", "modo", "n", "mediana (ms)", "ns por inteiro");
    bool tudo_certo = true;
    for (long long n : {1'000'000LL, 10'000'000LL}) {
        const fs::path dados = pasta / ("dados-" + std::to_string(n) + ".txt");
        const long long esperado = gerar(dados, n);
        if (n == 1'000'000) std::println("arquivo de 10^6 inteiros: {} bytes", fs::file_size(dados));
        for (const auto& c : casos) {
            if (c.n != n) continue;
            std::vector<double> ms;
            for (int rodada = 0; rodada < 6; ++rodada) {   // uma rodada de aquecimento e cinco medidas
                if (processo::executar(programa, c.modo, dados, saida, erros) != 0) tudo_certo = false;
                long long soma = 0, us = 0;
                std::sscanf(processo::conteudo(erros).c_str(), "%lld %lld", &soma, &us);
                if (soma != esperado) tudo_certo = false;
                if (rodada > 0) ms.push_back(us / 1000.0);
            }
            std::ranges::sort(ms);
            const double mediana = ms[ms.size() / 2];
            std::println("{:<11} | {:>9} | {:>12.3f} | {:>14.1f}", c.modo, n, mediana, mediana * 1e6 / n);
        }
        fs::remove(dados);
    }
    std::println("somas conferidas em todos os modos: {}", tudo_certo);
}
