// T02.1, as fileiras do pomar: somas de intervalo por prefixos.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> prefixo(n + 1, 0);        // até 2 * 10^5 * 10^9 = 2 * 10^14
    for (int i = 0; i < n; ++i) {
        long long caixas = 0;
        std::cin >> caixas;
        prefixo[i + 1] = prefixo[i] + caixas;
    }
    while (q-- > 0) {
        int l = 0, r = 0;
        std::cin >> l >> r;
        std::println("{}", prefixo[r] - prefixo[l - 1]);
    }
}
