// Leitor de inteiros com buffer, do Artigo 7: fread em blocos de 64 KiB e conversão por algarismos.
#pragma once
#include <cstdio>

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
    long long ler() {                                // valores até 18 algarismos, com sinal opcional
        int c = proximo();
        while (c != -1 && c != '-' && (c < '0' || c > '9')) c = proximo();
        const bool negativo = c == '-';
        if (negativo) c = proximo();
        long long v = 0;
        while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = proximo(); }
        return negativo ? -v : v;
    }
};
