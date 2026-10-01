// Rastreamentos pequenos: contagem estável, radix LSD em base 10, a passada instável e as chaves com sinal.
#include <array>
#include <algorithm>
#include <cstdint>
#include <print>
#include <random>
#include <string>
#include <vector>

struct Item {
    int chave;
    char rotulo;
};

void mostrar(const char* titulo, const std::vector<int>& v) {
    std::print("{:<28}:", titulo);
    for (int x : v) std::print(" {:03}", x);
    std::println();
}

// Uma passada de distribuição pelo dígito decimal de peso 'peso'. Se 'estavel' for falso,
// a passada percorre a entrada da esquerda para a direita, mas ainda preenche cada faixa do fim
// para o começo, e os empatados saem invertidos.
std::vector<int> passada_decimal(const std::vector<int>& a, int peso, bool estavel) {
    std::array<int, 10> cont{};
    for (int x : a) ++cont[(x / peso) % 10];
    for (int d = 1; d < 10; ++d) cont[d] += cont[d - 1];
    std::vector<int> saida(a.size());
    const int n = static_cast<int>(a.size());
    if (estavel)
        for (int i = n - 1; i >= 0; --i) saida[--cont[(a[i] / peso) % 10]] = a[i];
    else
        for (int i = 0; i < n; ++i) saida[--cont[(a[i] / peso) % 10]] = a[i];
    return saida;
}

std::uint32_t chave(std::int32_t x) { return static_cast<std::uint32_t>(x) ^ 0x80000000u; }

void radix_u32(std::vector<std::uint32_t>& a) {
    std::vector<std::uint32_t> tmp(a.size());
    for (int passada = 0; passada < 4; ++passada) {
        const int desloc = passada * 8;
        std::array<int, 256> cont{};
        for (auto x : a) ++cont[(x >> desloc) & 0xFFu];
        for (int i = 1; i < 256; ++i) cont[i] += cont[i - 1];
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) tmp[--cont[(a[i] >> desloc) & 0xFFu]] = a[i];
        a.swap(tmp);
    }
}

int main() {
    // 1. Contagem estável com rótulos.
    const std::vector<Item> itens{{4, 'a'}, {1, 'b'}, {3, 'c'}, {1, 'd'}, {0, 'e'}, {4, 'f'}, {1, 'g'}};
    std::array<int, 5> cont{};
    for (const auto& it : itens) ++cont[it.chave];
    std::print("contagem:");
    for (int c : cont) std::print(" {}", c);
    std::println();
    for (int i = 1; i < 5; ++i) cont[i] += cont[i - 1];
    std::print("prefixos:");
    for (int c : cont) std::print(" {}", c);
    std::println();
    std::vector<Item> saida(itens.size(), Item{-1, '.'});
    for (int i = static_cast<int>(itens.size()) - 1; i >= 0; --i) {
        const int pos = --cont[itens[i].chave];
        saida[pos] = itens[i];
        std::print("i = {}: {}{} vai para a posição {}, saída:", i, itens[i].rotulo, itens[i].chave, pos);
        for (const auto& s : saida) {
            if (s.chave < 0) std::print(" __");
            else std::print(" {}{}", s.rotulo, s.chave);
        }
        std::println();
    }

    // 2. Radix LSD em base 10, estável e instável.
    const std::vector<int> numeros{283, 917, 46, 512, 280, 95, 913};
    for (bool estavel : {true, false}) {
        std::println("{}", estavel ? "radix decimal com passadas estáveis" : "radix decimal com passadas instáveis");
        std::vector<int> v = numeros;
        mostrar("  entrada", v);
        const char* nomes[] = {"  depois das unidades", "  depois das dezenas", "  depois das centenas"};
        for (int p = 0, peso = 1; p < 3; ++p, peso *= 10) {
            v = passada_decimal(v, peso, estavel);
            mostrar(nomes[p], v);
        }
        std::println("  ordenado: {}", std::ranges::is_sorted(v));
    }

    // 3. Chaves com sinal.
    for (std::int32_t x : {-2147483647 - 1, -5, -1, 0, 3, 2147483647})
        std::println("x = {:>11}: bits {:08X}, chave {:08X}", x, static_cast<std::uint32_t>(x), chave(x));
    std::mt19937 gerador(7);
    std::uniform_int_distribution<std::int32_t> dist(-2147483647 - 1, 2147483647);
    std::vector<std::int32_t> s(1'000'000);
    for (auto& x : s) x = dist(gerador);
    std::vector<std::uint32_t> sem_inverter(s.size()), invertido(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        sem_inverter[i] = static_cast<std::uint32_t>(s[i]);
        invertido[i] = chave(s[i]);
    }
    radix_u32(sem_inverter);
    radix_u32(invertido);
    std::vector<std::int32_t> volta1(s.size()), volta2(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        volta1[i] = static_cast<std::int32_t>(sem_inverter[i]);
        volta2[i] = static_cast<std::int32_t>(invertido[i] ^ 0x80000000u);
    }
    std::ranges::sort(s);
    std::println("10^6 inteiros com sinal, sem inverter o bit: ordenado {}, primeiro {}", volta1 == s, volta1.front() >= 0);
    std::println("10^6 inteiros com sinal, com o bit invertido: ordenado {}", volta2 == s);
}
