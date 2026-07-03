#include <array>
#include <iostream>
#include <string>
#include <vector>

struct PrefixNode {
    std::array<int, 26> next;
    int words_below = 0;

    PrefixNode() {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int dictionary_size;
    int query_count;
    std::cin >> dictionary_size >> query_count;

    std::vector<PrefixNode> trie(1);
    while (dictionary_size-- > 0) {
        std::string word;
        std::cin >> word;
        int node = 0;
        for (char symbol : word) {
            const int letter = symbol - 'a';
            if (trie[node].next[letter] == -1) {
                trie[node].next[letter] = static_cast<int>(trie.size());
                trie.emplace_back();
            }
            node = trie[node].next[letter];
            ++trie[node].words_below;
        }
    }

    while (query_count-- > 0) {
        std::string prefix;
        std::cin >> prefix;
        int node = 0;
        for (char symbol : prefix) {
            const int letter = symbol - 'a';
            node = trie[node].next[letter];
            if (node == -1) {
                break;
            }
        }
        std::cout << (node == -1 ? 0 : trie[node].words_below) << '\n';
    }
}
