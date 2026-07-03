#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string inscription;
    std::cin >> inscription;

    const int n = static_cast<int>(inscription.size());
    std::vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int border = pi[i - 1];
        while (border > 0 && inscription[i] != inscription[border]) {
            border = pi[border - 1];
        }
        if (inscription[i] == inscription[border]) {
            ++border;
        }
        pi[i] = border;
    }

    const int candidate = n - pi.back();
    const int period = (n % candidate == 0) ? candidate : n;
    std::cout << inscription.substr(0, period) << '\n';
}
