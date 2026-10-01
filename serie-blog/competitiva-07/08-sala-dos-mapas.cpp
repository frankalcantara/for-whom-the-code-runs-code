// A sala dos mapas: soma dos bytes de um arquivo lido com fread, com ifstream e mapeado na memória.
#define _CRT_SECURE_NO_WARNINGS   // o MSVC marca fopen e scanf como inseguras (aviso C4996)
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <print>
#include <random>
#include <stdexcept>
#include <vector>
#include "medicao.hpp"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

unsigned long long somar(const unsigned char* p, std::size_t n) {
    unsigned long long s = 0;
    for (std::size_t i = 0; i < n; ++i) s += p[i];
    return s;
}

unsigned long long por_fread(const fs::path& caminho) {
    std::FILE* f = std::fopen(caminho.string().c_str(), "rb");
    if (!f) throw std::runtime_error("não foi possível abrir o arquivo");
    static std::array<unsigned char, 1 << 20> bloco;         // 1 MiB por chamada
    unsigned long long s = 0;
    for (std::size_t lidos; (lidos = std::fread(bloco.data(), 1, bloco.size(), f)) > 0;) s += somar(bloco.data(), lidos);
    std::fclose(f);
    return s;
}

unsigned long long por_ifstream(const fs::path& caminho) {
    std::ifstream in(caminho, std::ios::binary);
    static std::array<char, 1 << 20> bloco;
    unsigned long long s = 0;
    while (in.read(bloco.data(), bloco.size()) || in.gcount() > 0)
        s += somar(reinterpret_cast<const unsigned char*>(bloco.data()), static_cast<std::size_t>(in.gcount()));
    return s;
}

// Mapeia o arquivo inteiro no espaço de endereços do processo e percorre os bytes como um arranjo.
unsigned long long por_mapeamento(const fs::path& caminho) {
#ifdef _WIN32
    HANDLE arquivo = CreateFileW(caminho.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                                 FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    if (arquivo == INVALID_HANDLE_VALUE) throw std::runtime_error("CreateFileW falhou");
    LARGE_INTEGER tamanho{};
    GetFileSizeEx(arquivo, &tamanho);
    if (tamanho.QuadPart == 0) { CloseHandle(arquivo); return 0; }
    HANDLE mapa = CreateFileMappingW(arquivo, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!mapa) { CloseHandle(arquivo); throw std::runtime_error("CreateFileMappingW falhou"); }
    const auto* dados = static_cast<const unsigned char*>(MapViewOfFile(mapa, FILE_MAP_READ, 0, 0, 0));
    if (!dados) { CloseHandle(mapa); CloseHandle(arquivo); throw std::runtime_error("MapViewOfFile falhou"); }
    const unsigned long long s = somar(dados, static_cast<std::size_t>(tamanho.QuadPart));
    UnmapViewOfFile(dados);
    CloseHandle(mapa);
    CloseHandle(arquivo);
    return s;
#else
    const int fd = open(caminho.c_str(), O_RDONLY);
    if (fd < 0) throw std::runtime_error("open falhou");
    struct stat st{};
    fstat(fd, &st);
    if (st.st_size == 0) { close(fd); return 0; }
    void* p = mmap(nullptr, static_cast<std::size_t>(st.st_size), PROT_READ, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED) { close(fd); throw std::runtime_error("mmap falhou"); }
    const unsigned long long s = somar(static_cast<const unsigned char*>(p), static_cast<std::size_t>(st.st_size));
    munmap(p, static_cast<std::size_t>(st.st_size));
    close(fd);
    return s;
#endif
}

int main() {
    const fs::path pasta = fs::temp_directory_path() / "competitiva07-mapas";
    fs::create_directories(pasta);

    // Caso do enunciado: os bytes A, B e C somam 65 + 66 + 67 = 198.
    const fs::path letras = pasta / "letras.txt";
    std::ofstream(letras, std::ios::binary) << "ABC";
    std::println("ABC: fread={} ifstream={} mapeamento={}", por_fread(letras), por_ifstream(letras), por_mapeamento(letras));
    const fs::path vazio = pasta / "vazio.bin";
    std::ofstream(vazio, std::ios::binary).flush();
    std::println("vazio: fread={} mapeamento={}", por_fread(vazio), por_mapeamento(vazio));

    // Arquivo de 256 MiB de bytes pseudoaleatórios, lido com o cache de páginas já aquecido.
    const fs::path grande = pasta / "mapa.bin";
    constexpr std::size_t MiB = 1 << 20, total = 256 * MiB;
    unsigned long long esperado = 0;
    {
        std::mt19937_64 gerador(20260930);
        std::vector<unsigned char> bloco(MiB);
        std::FILE* f = std::fopen(grande.string().c_str(), "wb");
        for (std::size_t feito = 0; feito < total; feito += MiB) {
            for (std::size_t i = 0; i < MiB; i += 8) {
                const auto r = gerador();
                for (int b = 0; b < 8; ++b) bloco[i + b] = static_cast<unsigned char>(r >> (8 * b));
            }
            esperado += somar(bloco.data(), MiB);
            std::fwrite(bloco.data(), 1, MiB, f);
        }
        std::fclose(f);
    }
    long long obs = 0;
    const auto a = medir([&] { return por_fread(grande); }, obs);
    const auto b = medir([&] { return por_ifstream(grande); }, obs);
    const auto c = medir([&] { return por_mapeamento(grande); }, obs);
    const bool iguais = por_fread(grande) == esperado && por_ifstream(grande) == esperado && por_mapeamento(grande) == esperado;
    std::println("256 MiB, cache aquecido, mediana de 5 rodadas");
    std::println("fread, blocos de 1 MiB    : {:8.3f} ms", a.mediana_ms);
    std::println("ifstream, blocos de 1 MiB : {:8.3f} ms", b.mediana_ms);
    std::println("mapeamento na memória     : {:8.3f} ms", c.mediana_ms);
    std::println("somas iguais nos três modos: {}", iguais);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
    fs::remove(letras);
    fs::remove(vazio);
    fs::remove(grande);
}
