// Templates, conceitos, referências de encaminhamento e o this explícito.
#include <concepts>
#include <functional>
#include <limits>
#include <print>
#include <string>
#include <type_traits>
#include <vector>

template <typename T>
T quadrado(T x) { return x * x; }

template <typename T>
requires std::integral<T>
T dif_abs(T a, T b) { return a < b ? b - a : a - b; }

template <typename T>
concept InteiroAritmeticoComSinal =
    std::signed_integral<T> &&
    !std::same_as<std::remove_cvref_t<T>, char> &&
    !std::same_as<std::remove_cvref_t<T>, signed char> &&
    !std::same_as<std::remove_cvref_t<T>, wchar_t>;

template <InteiroAritmeticoComSinal T>
long long dif_abs_ll(T a, T b) {
    const long long x = a, y = b;
    return x < y ? y - x : x - y;
}

static_assert(std::integral<bool> && std::integral<char>);          // integral aceita ambos
static_assert(!InteiroAritmeticoComSinal<bool>);
static_assert(!InteiroAritmeticoComSinal<char>);
static_assert(!InteiroAritmeticoComSinal<unsigned>);
static_assert(InteiroAritmeticoComSinal<int> && InteiroAritmeticoComSinal<long long>);

template <typename Funcao>
requires std::invocable<Funcao&>
int chamar_tres_vezes(Funcao&& f) { return f() + f() + f(); }

static_assert(std::invocable<decltype([] { return 1; })&>);
static_assert(!std::invocable<int&>);

int main() {
    std::println("quadrado(7) = {}, quadrado(7LL) = {}, quadrado(1.5) = {}", quadrado(7), quadrado(7LL), quadrado(1.5));
    std::println("quadrado(100000LL) = {}", quadrado(100'000LL));
    std::println("dif_abs(3, 10) = {}, dif_abs(10u, 3u) = {}", dif_abs(3, 10), dif_abs(10u, 3u));
    const int menor = std::numeric_limits<int>::min(), maior = std::numeric_limits<int>::max();
    std::println("dif_abs_ll(INT_MIN, INT_MAX) = {}", dif_abs_ll(menor, maior));

    int contador = 0;
    std::println("chamar_tres_vezes = {}", chamar_tres_vezes([&] { return ++contador; }));

    // Lambda recursiva com this explícito (deducing this).
    auto fib = [](this auto&& self, int n) -> long long { return n < 2 ? n : self(n - 1) + self(n - 2); };
    std::println("fib(10) = {}", fib(10));

    // Busca em profundidade local sobre uma árvore pequena.
    const std::vector<std::vector<int>> adj{{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    std::string ordem;
    auto dfs = [&](this auto&& self, int u, int pai) -> void {
        ordem += std::to_string(u) + ' ';
        for (int v : adj[u])
            if (v != pai) self(v, u);
    };
    dfs(0, -1);
    std::println("ordem da dfs: {}", ordem);
}
