// Vistas preguiçosas, algoritmos de ranges e projeções.
#include <algorithm>
#include <functional>
#include <iterator>
#include <print>
#include <ranges>
#include <span>
#include <string>
#include <vector>

struct Aluno {
    std::string nome;
    int nota;
};

int main() {
    std::vector<int> valores{7, 2, 9, 4, 1, 8, 6, 3, 10, 12};

    // 1. Uma vista é uma receita: o filtro só roda quando alguém pede elementos.
    int testes = 0;
    auto resultado = valores
        | std::views::filter([&](int x) { ++testes; return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);
    std::println("testes do filtro antes de percorrer: {}", testes);
    std::print("resultado:");
    for (int x : resultado) std::print(" {}", x);
    std::println("");
    std::println("testes do filtro depois de percorrer: {}", testes);

    std::span<const int> tudo(valores);
    auto pares = tudo | std::views::filter([](int x) { return x % 2 == 0; });
    std::println("quantidade de pares = {}", std::ranges::distance(pares));

    // 2. enumerate antes de stride preserva o índice original.
    const std::vector<int> inscricoes{10, 20, 30, 40, 50, 60, 70, 80};
    std::print("enumerate | stride(3):");
    for (auto [i, x] : std::views::enumerate(inscricoes) | std::views::stride(3)) std::print(" ({}, {})", i, x);
    std::println("");
    std::print("stride(3) | enumerate:");
    for (auto [i, x] : inscricoes | std::views::stride(3) | std::views::enumerate) std::print(" ({}, {})", i, x);
    std::println("");

    // 3. ordenar, unique e erase.
    std::vector<int> r{5, 1, 3, 1, 5, 5, 2, 3};
    std::ranges::sort(r);
    auto [primeiro_removido, fim] = std::ranges::unique(r);
    std::println("depois de unique: tamanho {}, cauda com {} elementos", r.size(),
                 std::ranges::distance(primeiro_removido, fim));
    r.erase(primeiro_removido, fim);
    std::print("depois de erase:");
    for (int x : r) std::print(" {}", x);
    std::println("");

    // 4. Projeções.
    std::vector<Aluno> turma{{"Bia", 710}, {"Caio", 640}, {"Ana", 820}, {"Davi", 700}, {"Eva", 640}};
    std::ranges::sort(turma, {}, &Aluno::nota);
    std::print("por nota crescente:");
    for (const auto& a : turma) std::print(" {}:{}", a.nome, a.nota);
    std::println("");
    auto it = std::ranges::lower_bound(turma, 700, {}, &Aluno::nota);
    std::println("primeiro com nota >= 700: {} ({})", it->nome, it->nota);
    std::ranges::sort(turma, std::greater<>{}, &Aluno::nota);
    std::print("por nota decrescente:");
    for (const auto& a : turma) std::print(" {}:{}", a.nome, a.nota);
    std::println("");

    // 5. copy_if e count_if.
    std::vector<int> grandes;
    std::ranges::copy_if(valores, std::back_inserter(grandes), [](int x) { return x > 6; });
    std::print("maiores que 6:");
    for (int x : grandes) std::print(" {}", x);
    std::println("");
    std::println("count_if(x % 3 == 0) = {}", std::ranges::count_if(valores, [](int x) { return x % 3 == 0; }));
}
