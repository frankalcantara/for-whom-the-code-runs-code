#include <array>
#include <iostream>
#include <string>
#include <vector>

struct PalindromeNode {
    std::array<int, 26> next;
    int length = 0;
    int suffix_link = 0;

    explicit PalindromeNode(int palindrome_length = 0)
        : length(palindrome_length) {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;

    std::vector<PalindromeNode> tree;
    tree.emplace_back(-1);
    tree.emplace_back(0);
    tree[0].suffix_link = 0;
    tree[1].suffix_link = 0;

    int longest_suffix = 1;
    for (int position = 0; position < static_cast<int>(text.size());
         ++position) {
        const int letter = text[position] - 'a';
        int candidate = longest_suffix;

        while (position - 1 - tree[candidate].length < 0 ||
               text[position - 1 - tree[candidate].length] !=
                   text[position]) {
            candidate = tree[candidate].suffix_link;
        }

        if (tree[candidate].next[letter] != -1) {
            longest_suffix = tree[candidate].next[letter];
            continue;
        }

        const int created = static_cast<int>(tree.size());
        tree.emplace_back(tree[candidate].length + 2);
        tree[candidate].next[letter] = created;

        if (tree[created].length == 1) {
            tree[created].suffix_link = 1;
        } else {
            int link_candidate = tree[candidate].suffix_link;
            while (position - 1 - tree[link_candidate].length < 0 ||
                   text[position - 1 - tree[link_candidate].length] !=
                       text[position]) {
                link_candidate = tree[link_candidate].suffix_link;
            }
            tree[created].suffix_link =
                tree[link_candidate].next[letter];
        }
        longest_suffix = created;
    }

    std::cout << tree.size() - 2 << '\n';
}
