#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::string pattern;
    std::cin >> text >> pattern;

    std::vector<int> pi(pattern.size());
    for (std::size_t i = 1; i < pattern.size(); ++i) {
        int border = pi[i - 1];
        while (border > 0 && pattern[i] != pattern[border]) {
            border = pi[border - 1];
        }
        if (pattern[i] == pattern[border]) {
            ++border;
        }
        pi[i] = border;
    }

    long long occurrences = 0;
    int matched = 0;
    for (char symbol : text) {
        while (matched > 0 && symbol != pattern[matched]) {
            matched = pi[matched - 1];
        }
        if (symbol == pattern[matched]) {
            ++matched;
        }
        if (matched == static_cast<int>(pattern.size())) {
            ++occurrences;
            matched = pi[matched - 1];
        }
    }

    std::cout << occurrences << '\n';
}
