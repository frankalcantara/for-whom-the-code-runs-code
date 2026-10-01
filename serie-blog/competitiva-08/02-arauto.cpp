// T03.2, o fôlego do arauto: contar palavras e achar a primeira mais longa sem copiar texto.
#include <iostream>
#include <print>
#include <string>
#include <string_view>

int main() {
    std::string linha;                               // a única dona dos caracteres
    std::getline(std::cin, linha);
    if (!linha.empty() && linha.back() == '\r') linha.pop_back();   // aceita quebras do Windows
    const std::string_view texto{linha};

    long long palavras = 0;
    std::string_view mais_longa{};
    std::size_t i = 0;
    while (i < texto.size()) {
        while (i < texto.size() && texto[i] == ' ') ++i;      // pula a sequência de espaços
        if (i == texto.size()) break;
        const std::size_t inicio = i;
        while (i < texto.size() && texto[i] != ' ') ++i;      // percorre a palavra
        const std::string_view palavra = texto.substr(inicio, i - inicio);
        ++palavras;
        if (palavra.size() > mais_longa.size()) mais_longa = palavra;   // estritamente maior: a primeira vence
    }
    std::println("{} {}", palavras, mais_longa);
}
