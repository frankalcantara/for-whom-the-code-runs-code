// Execução de um processo filho com entrada e saídas redirecionadas para arquivos,
// usada pelos programas de medição de entrada e saída.
#pragma once
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace processo {

inline std::string aspas(const std::filesystem::path& p) { return "\"" + p.string() + "\""; }

// Executa: programa argumentos < entrada > saida 2> erros. Devolve o código de saída.
inline int executar(const std::filesystem::path& programa, const std::string& argumentos,
                    const std::filesystem::path& entrada, const std::filesystem::path& saida,
                    const std::filesystem::path& erros) {
    std::string cmd = aspas(programa) + " " + argumentos;
    if (!entrada.empty()) cmd += " < " + aspas(entrada);
    cmd += " > " + aspas(saida) + " 2> " + aspas(erros);
#ifdef _WIN32
    cmd = "\"" + cmd + "\"";          // o cmd.exe remove o primeiro e o último caractere de aspas
#endif
    return std::system(cmd.c_str());
}

inline std::string conteudo(const std::filesystem::path& p) {
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

}  // namespace processo
