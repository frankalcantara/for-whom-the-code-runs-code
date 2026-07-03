#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;

    const int n = static_cast<int>(text.size());
    std::vector<int> z(n);
    int left = 0;
    int right = 0;

    for (int i = 1; i < n; ++i) {
        if (i < right) {
            z[i] = std::min(right - i, z[i - left]);
        }
        while (i + z[i] < n && text[z[i]] == text[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
    }

    for (int i = 0; i < n; ++i) {
        std::cout << z[i] << " \n"[i + 1 == n];
    }
}
