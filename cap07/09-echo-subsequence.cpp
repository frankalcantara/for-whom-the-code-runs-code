#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string transmission;
    std::string echo;
    std::cin >> transmission >> echo;

    const int n = static_cast<int>(transmission.size());
    const int m = static_cast<int>(echo.size());
    int answer = 0;

    for (int start = 0; start + m <= n; ++start) {
        const int finish = std::min(n, start + 2 * m);
        int matched = 0;
        for (int position = start;
             position < finish && matched < m;
             ++position) {
            if (transmission[static_cast<std::size_t>(position)]
                == echo[static_cast<std::size_t>(matched)]) {
                ++matched;
            }
        }
        answer += matched == m;
    }

    std::cout << answer << '\n';
}
