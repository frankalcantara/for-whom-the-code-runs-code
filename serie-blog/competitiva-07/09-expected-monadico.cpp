// Operações monádicas de std::expected: and_then, transform, or_else e transform_error.
#include <charconv>
#include <expected>
#include <print>
#include <string>
#include <string_view>

enum class Erro { vazio, nao_numero, negativo };

std::string_view nome(Erro e) {
    switch (e) {
        case Erro::vazio: return "entrada vazia";
        case Erro::nao_numero: return "não é um número";
        case Erro::negativo: return "valor negativo";
    }
    return "?";
}

std::expected<std::string_view, Erro> nao_vazio(std::string_view s) {
    if (s.empty()) return std::unexpected(Erro::vazio);
    return s;
}

std::expected<long long, Erro> converter(std::string_view s) {
    long long v = 0;
    const auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), v);
    if (ec != std::errc{} || p != s.data() + s.size()) return std::unexpected(Erro::nao_numero);
    return v;
}

std::expected<long long, Erro> nao_negativo(long long v) {
    if (v < 0) return std::unexpected(Erro::negativo);
    return v;
}

// Escada de retornos antecipados: a forma portátil.
std::expected<long long, Erro> dobro_escada(std::string_view s) {
    auto a = nao_vazio(s);
    if (!a) return std::unexpected(a.error());
    auto b = converter(*a);
    if (!b) return std::unexpected(b.error());
    auto c = nao_negativo(*b);
    if (!c) return std::unexpected(c.error());
    return 2 * *c;
}

// A mesma cadeia com as operações monádicas do C++23.
std::expected<long long, Erro> dobro_cadeia(std::string_view s) {
    return nao_vazio(s)
        .and_then(converter)                          // etapa que pode falhar
        .and_then(nao_negativo)                       // outra etapa que pode falhar
        .transform([](long long v) { return 2 * v; });   // etapa que não falha
}

int main() {
    for (std::string_view s : {"21", "", "4x", "-5"}) {
        const auto a = dobro_escada(s), b = dobro_cadeia(s);
        const std::string texto = b ? std::to_string(*b) : std::string("erro: ") + std::string(nome(b.error()));
        std::println("'{}' -> {} (escada e cadeia iguais: {})", s, texto, a == b);
    }
    // or_else recupera de uma falha; transform_error traduz o erro para o domínio de quem chama.
    const long long padrao = dobro_cadeia("4x").or_else([](Erro) { return std::expected<long long, Erro>{0}; }).value();
    std::println("or_else com padrão 0: {}", padrao);
    const auto traduzido = dobro_cadeia("").transform_error([](Erro e) { return std::string("configuração inválida: ") + std::string(nome(e)); });
    std::println("transform_error: {}", traduzido.error());
    std::println("value_or: {}", dobro_cadeia("-5").value_or(-1));
}
