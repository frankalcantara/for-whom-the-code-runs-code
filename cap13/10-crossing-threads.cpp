#include <algorithm>
#include <iostream>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(int size) : tree_(size + 1) {}

    void add(int index, int delta) {
        for (; index < static_cast<int>(tree_.size());
             index += index & -index) {
            tree_[index] += delta;
        }
    }

    int prefix_sum(int index) const {
        int result = 0;
        for (; index > 0; index -= index & -index) {
            result += tree_[index];
        }
        return result;
    }

    int range_sum(int left, int right) const {
        return prefix_sum(right) - prefix_sum(left - 1);
    }

private:
    std::vector<int> tree_;
};

struct Event {
    long long x;
    int kind;
    long long y1;
    long long y2;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int segment_count;
    std::cin >> segment_count;

    std::vector<Event> events;
    std::vector<long long> coordinates;
    events.reserve(segment_count * 2);
    coordinates.reserve(segment_count * 2);

    for (int i = 0; i < segment_count; ++i) {
        long long x1;
        long long y1;
        long long x2;
        long long y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        if (y1 == y2) {
            if (x1 > x2) {
                std::swap(x1, x2);
            }
            events.push_back({x1, 0, y1, y1});
            events.push_back({x2, 2, y1, y1});
            coordinates.push_back(y1);
        } else {
            if (y1 > y2) {
                std::swap(y1, y2);
            }
            events.push_back({x1, 1, y1, y2});
            coordinates.push_back(y1);
            coordinates.push_back(y2);
        }
    }

    std::ranges::sort(coordinates);
    coordinates.erase(
        std::unique(coordinates.begin(), coordinates.end()),
        coordinates.end());
    std::ranges::sort(events, [](const Event& first, const Event& second) {
        return first.x != second.x ? first.x < second.x
                                   : first.kind < second.kind;
    });

    FenwickTree active(static_cast<int>(coordinates.size()));
    long long intersections = 0;

    for (const Event& event : events) {
        if (event.kind == 0 || event.kind == 2) {
            const int y =
                std::ranges::lower_bound(coordinates, event.y1) -
                coordinates.begin() + 1;
            active.add(y, event.kind == 0 ? 1 : -1);
            continue;
        }

        const int bottom =
            std::ranges::lower_bound(coordinates, event.y1) -
            coordinates.begin() + 1;
        const int top =
            std::ranges::upper_bound(coordinates, event.y2) -
            coordinates.begin();
        intersections += active.range_sum(bottom, top);
    }

    std::cout << intersections << '\n';
}
