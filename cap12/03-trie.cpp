// cap12/03-trie.cpp
// Prefix trie for strings and bitwise trie for maximum XOR queries.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o trie 03-trie.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetrie.exe 03-trie.cpp

#include <array>
#include <cassert>
#include <print>
#include <string>
#include <vector>

struct Trie {
    static constexpr int ALPHA = 26;
    std::vector<std::array<int, ALPHA>> ch;
    std::vector<int> cnt, end_;

    Trie() {
        ch.push_back({});
        ch[0].fill(-1);
        cnt.push_back(0);
        end_.push_back(0);
    }

    void insert(const std::string& s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (ch[cur][idx] == -1) {
                ch[cur][idx] = static_cast<int>(ch.size());
                ch.push_back({});
                ch.back().fill(-1);
                cnt.push_back(0);
                end_.push_back(0);
            }
            cur = ch[cur][idx];
            ++cnt[cur];
        }
        ++end_[cur];
    }

    bool search(const std::string& s) const {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (ch[cur][idx] == -1) return false;
            cur = ch[cur][idx];
        }
        return end_[cur] > 0;
    }

    int count_prefix(const std::string& p) const {
        int cur = 0;
        for (char c : p) {
            int idx = c - 'a';
            if (ch[cur][idx] == -1) return 0;
            cur = ch[cur][idx];
        }
        return cnt[cur];
    }
};

struct XorTrie {
    static constexpr int BITS = 30;
    std::vector<std::array<int, 2>> ch;

    XorTrie() {
        ch.push_back({-1, -1});
    }

    void insert(int x) {
        int cur = 0;
        for (int b = BITS; b >= 0; --b) {
            int bit = (x >> b) & 1;
            if (ch[cur][bit] == -1) {
                ch[cur][bit] = static_cast<int>(ch.size());
                ch.push_back({-1, -1});
            }
            cur = ch[cur][bit];
        }
    }

    int max_xor(int x) const {
        int cur = 0, result = 0;
        for (int b = BITS; b >= 0; --b) {
            int bit = (x >> b) & 1;
            int want = 1 - bit;
            if (ch[cur][want] != -1) {
                result |= (1 << b);
                cur = ch[cur][want];
            } else if (ch[cur][bit] != -1) {
                cur = ch[cur][bit];
            } else {
                break;
            }
        }
        return result;
    }
};

int main() {
    Trie t;
    t.insert("apple");
    t.insert("app");
    t.insert("application");
    t.insert("banana");

    assert(t.search("apple"));
    assert(t.search("app"));
    assert(!t.search("ap"));
    assert(!t.search("appl"));
    assert(t.search("banana"));

    assert(t.count_prefix("app") == 3);
    assert(t.count_prefix("ban") == 1);
    assert(t.count_prefix("c") == 0);

    XorTrie xt;
    xt.insert(3);
    xt.insert(10);
    xt.insert(5);

    assert(xt.max_xor(7) == 13);
    assert(xt.max_xor(0) == 10);

    std::print("03-trie: all assertions passed\n");
    return 0;
}
