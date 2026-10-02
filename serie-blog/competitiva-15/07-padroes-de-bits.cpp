// Os padrões de bits de 0,1 + 0,2, de 0,3 e dos dois zeros, com std::bit_cast, e uma troca de bytes com std::byteswap.
#include <bit>
#include <cstdint>
#include <print>

int main() {
    const double soma = 0.1 + 0.2, tres = 0.3;
    const auto ps = std::bit_cast<std::uint64_t>(soma);
    const auto pt = std::bit_cast<std::uint64_t>(tres);
    std::println("0.1 + 0.2 = {:016x}", ps);
    std::println("0.3       = {:016x}", pt);
    std::println("diferença em unidades da última casa: {}", ps - pt);
    std::println("+0.0 = {:016x}", std::bit_cast<std::uint64_t>(+0.0));
    std::println("-0.0 = {:016x}", std::bit_cast<std::uint64_t>(-0.0));
    std::println("+0.0 == -0.0: {}", +0.0 == -0.0);
    const std::uint32_t lido = 0x78563412u;                        // bytes 12 34 56 78 lidos em um processador little-endian
    std::println("{:08x} -> {:08x}", lido, std::byteswap(lido));
}
