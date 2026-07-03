#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string initial;
    std::string target;
    std::cin >> initial >> target;

    int answer = 0;
    for (std::size_t dial = 0; dial < initial.size(); ++dial) {
        const int direct = std::abs(initial[dial] - target[dial]);
        answer += std::min(direct, 10 - direct);
    }

    std::cout << answer << '\n';
}
