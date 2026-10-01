// A forma dos números: bases, sufixos, tipos deduzidos e literais reais.
#include <cstddef>
#include <cstdint>
#include <print>
#include <string_view>
#include <type_traits>

// Nome legível do tipo de uma expressão, sem depender do formato de typeid.
template <typename T>
constexpr std::string_view nome() {
    using U = std::remove_cvref_t<T>;
    if constexpr (std::is_same_v<U, int>) return "int";
    else if constexpr (std::is_same_v<U, unsigned>) return "unsigned int";
    else if constexpr (std::is_same_v<U, long>) return "long";
    else if constexpr (std::is_same_v<U, unsigned long>) return "unsigned long";
    else if constexpr (std::is_same_v<U, long long>) return "long long";
    else if constexpr (std::is_same_v<U, unsigned long long>) return "unsigned long long";
    else if constexpr (std::is_same_v<U, float>) return "float";
    else if constexpr (std::is_same_v<U, double>) return "double";
    else return "outro";
}

#define MOSTRAR(expr) std::println("{:<24} valor {:<22} tipo {}", #expr, (expr), nome<decltype(expr)>())

int main() {
    MOSTRAR(42);
    MOSTRAR(010);                 // octal: vale 8
    MOSTRAR(0x1F);
    MOSTRAR(0b1010);
    MOSTRAR(2147483647);
    MOSTRAR(2147483648);          // não cabe em int
    MOSTRAR(0x80000000);          // hexadecimal pode ir para unsigned
    MOSTRAR(3'000'000'000);
    MOSTRAR(42U);
    MOSTRAR(42LL);
    MOSTRAR(1'000'000LL * 1'000'000);
    std::println("sizeof(long) = {} bytes", sizeof(long));
    std::println("42uz é std::size_t: {}", std::is_same_v<decltype(42uz), std::size_t>);
    std::println("42z tem sinal e 8 bytes: {}", std::is_signed_v<decltype(42z)> && sizeof(42z) == 8);
    std::println("0x1.Fp3 = {}", 0x1.Fp3);
    std::println("0.1 + 0.2 == 0.3: {}", 0.1 + 0.2 == 0.3);
    std::println("0.1 + 0.2 = {:.17g}", 0.1 + 0.2);
#if defined(__STDCPP_FLOAT16_T__)
    std::println("std::float16_t disponível: sim");
#else
    std::println("std::float16_t disponível: não");
#endif
}
