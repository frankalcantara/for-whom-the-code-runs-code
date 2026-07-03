#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    std::array<int, 26> next;
    int parent = 0;
    int depth = 0;

    Node() {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int word_count;
    int query_count;
    std::cin >> word_count >> query_count;

    std::vector<Node> trie(1);
    std::vector<int> terminal(word_count);

    for (int id = 0; id < word_count; ++id) {
        std::string word;
        std::cin >> word;
        int node = 0;
        for (char symbol : word) {
            const int letter = symbol - 'a';
            if (trie[node].next[letter] == -1) {
                trie[node].next[letter] = static_cast<int>(trie.size());
                Node child;
                child.parent = node;
                child.depth = trie[node].depth + 1;
                trie.push_back(child);
            }
            node = trie[node].next[letter];
        }
        terminal[id] = node;
    }

    const int nodes = static_cast<int>(trie.size());
    std::vector<int> subtree_size(nodes, 1);
    std::vector<int> heavy_child(nodes, -1);
    for (int node = nodes - 1; node > 0; --node) {
        const int parent = trie[node].parent;
        subtree_size[parent] += subtree_size[node];
        if (heavy_child[parent] == -1 ||
            subtree_size[node] > subtree_size[heavy_child[parent]]) {
            heavy_child[parent] = node;
        }
    }

    std::vector<int> chain_head(nodes);
    for (int node = 1; node < nodes; ++node) {
        const int parent = trie[node].parent;
        chain_head[node] =
            (heavy_child[parent] == node) ? chain_head[parent] : node;
    }

    auto lca = [&](int first, int second) {
        while (chain_head[first] != chain_head[second]) {
            if (trie[chain_head[first]].depth <
                trie[chain_head[second]].depth) {
                std::swap(first, second);
            }
            first = trie[chain_head[first]].parent;
        }
        return trie[first].depth < trie[second].depth ? first : second;
    };

    while (query_count-- > 0) {
        int first;
        int second;
        std::cin >> first >> second;
        --first;
        --second;
        std::cout << trie[lca(terminal[first], terminal[second])].depth
                  << '\n';
    }
}
