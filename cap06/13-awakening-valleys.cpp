#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class ActiveDisjointSet {
public:
    explicit ActiveDisjointSet(int n)
        : parent_(static_cast<std::size_t>(n), -1),
          size_(static_cast<std::size_t>(n), 1) {}

    bool active(int x) const {
        return parent_[static_cast<std::size_t>(x)] != -1;
    }

    void activate(int x) {
        parent_[static_cast<std::size_t>(x)] = x;
        ++components_;
    }

    int find(int x) {
        if (parent_[static_cast<std::size_t>(x)] != x) {
            parent_[static_cast<std::size_t>(x)] =
                find(parent_[static_cast<std::size_t>(x)]);
        }
        return parent_[static_cast<std::size_t>(x)];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size_[static_cast<std::size_t>(a)]
            < size_[static_cast<std::size_t>(b)]) {
            std::swap(a, b);
        }
        parent_[static_cast<std::size_t>(b)] = a;
        size_[static_cast<std::size_t>(a)] +=
            size_[static_cast<std::size_t>(b)];
        --components_;
    }

    int components() const {
        return components_;
    }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
    int components_ = 0;
};

struct Question {
    int water_level;
    int original_index;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int positions = 0;
    int question_count = 0;
    std::cin >> positions >> question_count;

    std::vector<int> elevation(static_cast<std::size_t>(positions));
    for (int& value : elevation) {
        std::cin >> value;
    }

    std::vector<int> order(static_cast<std::size_t>(positions));
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, {}, [&](int index) {
        return elevation[static_cast<std::size_t>(index)];
    });

    std::vector<Question> questions(static_cast<std::size_t>(question_count));
    for (int i = 0; i < question_count; ++i) {
        std::cin >> questions[static_cast<std::size_t>(i)].water_level;
        questions[static_cast<std::size_t>(i)].original_index = i;
    }
    std::ranges::sort(questions, {}, &Question::water_level);

    ActiveDisjointSet valleys(positions);
    std::vector<int> answer(static_cast<std::size_t>(question_count));
    int next_position = 0;

    for (const Question& question : questions) {
        while (next_position < positions
               && elevation[static_cast<std::size_t>(
                      order[static_cast<std::size_t>(next_position)])]
                      < question.water_level) {
            const int position =
                order[static_cast<std::size_t>(next_position++)];
            valleys.activate(position);
            if (position > 0 && valleys.active(position - 1)) {
                valleys.unite(position, position - 1);
            }
            if (position + 1 < positions && valleys.active(position + 1)) {
                valleys.unite(position, position + 1);
            }
        }
        answer[static_cast<std::size_t>(question.original_index)] =
            valleys.components();
    }

    for (int value : answer) {
        std::cout << value << '\n';
    }
}
