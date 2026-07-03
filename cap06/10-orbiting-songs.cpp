#include <iostream>
#include <iterator>
#include <set>
#include <vector>

class LowerMedian {
public:
    void insert(long long value) {
        if (lower_.empty() || value <= *lower_.rbegin()) {
            lower_.insert(value);
        } else {
            upper_.insert(value);
        }
        rebalance();
    }

    void erase_one(long long value) {
        auto lower_position = lower_.find(value);
        if (lower_position != lower_.end()) {
            lower_.erase(lower_position);
        } else {
            auto upper_position = upper_.find(value);
            upper_.erase(upper_position);
        }
        rebalance();
    }

    long long median() const {
        return *lower_.rbegin();
    }

private:
    void rebalance() {
        while (lower_.size() < upper_.size()) {
            auto first_upper = upper_.begin();
            lower_.insert(*first_upper);
            upper_.erase(first_upper);
        }
        while (lower_.size() > upper_.size() + 1) {
            auto last_lower = std::prev(lower_.end());
            upper_.insert(*last_lower);
            lower_.erase(last_lower);
        }
    }

    std::multiset<long long> lower_;
    std::multiset<long long> upper_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int songs = 0;
    int events = 0;
    std::cin >> songs >> events;

    std::vector<long long> plays(static_cast<std::size_t>(songs));
    LowerMedian ranking;
    for (long long& value : plays) {
        std::cin >> value;
        ranking.insert(value);
    }

    while (events-- > 0) {
        int song = 0;
        long long increase = 0;
        std::cin >> song >> increase;
        --song;

        ranking.erase_one(plays[static_cast<std::size_t>(song)]);
        plays[static_cast<std::size_t>(song)] += increase;
        ranking.insert(plays[static_cast<std::size_t>(song)]);
        std::cout << ranking.median() << '\n';
    }
}
