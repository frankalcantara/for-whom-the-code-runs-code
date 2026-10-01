// Estabilidade: o que o std::ranges::sort promete, o que o stable_sort promete
// e como várias passadas estáveis constroem uma ordem lexicográfica.
#include <algorithm>
#include <functional>
#include <print>
#include <random>
#include <string>
#include <tuple>
#include <vector>

struct Competidor {
    int id;
    int pontos;
    std::string nome;
};

// Pares vizinhos com a mesma chave e índices originais fora de ordem.
template <typename V>
int quebras(const V& v) {
    int q = 0;
    for (std::size_t i = 1; i < v.size(); ++i)
        if (v[i].pontos == v[i - 1].pontos && v[i].id < v[i - 1].id) ++q;
    return q;
}

void mostrar(const char* rotulo, const std::vector<Competidor>& v) {
    std::print("{}:", rotulo);
    for (const auto& c : v) std::print(" {}({})", c.nome, c.pontos);
    std::println("");
}

int main() {
    // 1. O exemplo de quatro competidores, na ordem de inscrição.
    const std::vector<Competidor> inscritos{{0, 80, "Ana"}, {1, 95, "Bruno"}, {2, 80, "Carla"}, {3, 95, "Diego"}};
    auto v = inscritos;
    std::ranges::stable_sort(v, std::greater<>{}, &Competidor::pontos);
    mostrar("stable_sort", v);
    v = inscritos;
    std::ranges::sort(v, std::greater<>{}, &Competidor::pontos);
    mostrar("sort       ", v);

    // 2. Mil competidores com dez pontuações possíveis.
    std::mt19937 gerador(20260930);
    std::uniform_int_distribution<int> dist(0, 9);
    std::vector<Competidor> mil(1000);
    for (int i = 0; i < 1000; ++i) mil[i] = {i, dist(gerador), ""};
    auto a = mil, b = mil;
    std::ranges::sort(a, {}, &Competidor::pontos);
    std::ranges::stable_sort(b, {}, &Competidor::pontos);
    std::println("1000 registros, 10 chaves: quebras de ordem original com sort {}, com stable_sort {}", quebras(a), quebras(b));

    // 3. Duas passadas estáveis: primeiro a chave secundária, depois a primária.
    std::vector<Competidor> cs{{1, 80, "Carlos"}, {2, 95, "Diana"}, {3, 80, "Bruno"}, {4, 72, "Alice"}, {5, 95, "Ana"}};
    std::ranges::stable_sort(cs, {}, &Competidor::nome);
    mostrar("passada 1, nome crescente      ", cs);
    std::ranges::stable_sort(cs, std::greater<>{}, &Competidor::pontos);
    mostrar("passada 2, pontos decrescentes ", cs);
    auto t = cs;
    std::ranges::sort(t, {}, [](const Competidor& c) { return std::tuple{-c.pontos, c.nome}; });
    std::println("mesma ordem da projeção para tupla: {}",
                 std::ranges::equal(t, cs, {}, &Competidor::id, &Competidor::id));
}
