#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

class DynamicBitset {
public:
    explicit DynamicBitset(std::size_t bit_count)
        : bit_count_(bit_count), words_((bit_count + 63) / 64, 0) {}

    void set(std::size_t index) {
        words_[index / 64] |= std::uint64_t{1} << (index % 64);
    }

    [[nodiscard]] bool test(std::size_t index) const {
        return (words_[index / 64]
                & (std::uint64_t{1} << (index % 64)))
               != 0;
    }

    void include_shifted(std::size_t shift) {
        const std::size_t whole_words = shift / 64;
        const std::size_t offset = shift % 64;

        for (std::size_t destination = words_.size(); destination-- > 0;) {
            std::uint64_t shifted = 0;
            if (destination >= whole_words) {
                shifted |= words_[destination - whole_words] << offset;
                if (offset != 0 && destination > whole_words) {
                    shifted |=
                        words_[destination - whole_words - 1]
                        >> (64 - offset);
                }
            }
            words_[destination] |= shifted;
        }

        const std::size_t valid = bit_count_ % 64;
        if (valid != 0) {
            words_.back() &= (std::uint64_t{1} << valid) - 1;
        }
    }

private:
    std::size_t bit_count_;
    std::vector<std::uint64_t> words_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int crates = 0;
    std::cin >> crates;

    std::vector<int> weight(static_cast<std::size_t>(crates));
    for (int& value : weight) {
        std::cin >> value;
    }

    const long long total =
        std::accumulate(weight.begin(), weight.end(), 0LL);
    if ((total & 1LL) != 0) {
        std::cout << "NO\n";
        return 0;
    }

    const std::size_t target = static_cast<std::size_t>(total / 2);
    DynamicBitset reachable(target + 1);
    reachable.set(0);

    for (int value : weight) {
        reachable.include_shifted(static_cast<std::size_t>(value));
    }

    std::cout << (reachable.test(target) ? "YES\n" : "NO\n");
}
