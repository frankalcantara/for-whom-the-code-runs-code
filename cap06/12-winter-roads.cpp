#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(int n)
        : parent_(static_cast<std::size_t>(n)),
          size_(static_cast<std::size_t>(n), 1),
          components_(n) {
        std::iota(parent_.begin(), parent_.end(), 0);
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
    int components_;
};

struct Road {
    int a;
    int b;
    long long resistance;
};

struct Question {
    long long storm;
    int original_index;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int villages = 0;
    int road_count = 0;
    int question_count = 0;
    std::cin >> villages >> road_count >> question_count;

    std::vector<Road> roads(static_cast<std::size_t>(road_count));
    for (Road& road : roads) {
        std::cin >> road.a >> road.b >> road.resistance;
        --road.a;
        --road.b;
    }

    std::vector<Question> questions(static_cast<std::size_t>(question_count));
    for (int i = 0; i < question_count; ++i) {
        std::cin >> questions[static_cast<std::size_t>(i)].storm;
        questions[static_cast<std::size_t>(i)].original_index = i;
    }

    std::ranges::sort(roads, std::greater{}, &Road::resistance);
    std::ranges::sort(questions, std::greater{}, &Question::storm);

    DisjointSet regions(villages);
    std::vector<int> answer(static_cast<std::size_t>(question_count));
    int next_road = 0;

    for (const Question& question : questions) {
        while (next_road < road_count
               && roads[static_cast<std::size_t>(next_road)].resistance
                      >= question.storm) {
            const Road& road = roads[static_cast<std::size_t>(next_road)];
            regions.unite(road.a, road.b);
            ++next_road;
        }
        answer[static_cast<std::size_t>(question.original_index)] =
            regions.components();
    }

    for (int value : answer) {
        std::cout << value << '\n';
    }
}
