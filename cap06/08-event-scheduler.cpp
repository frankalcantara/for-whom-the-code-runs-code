// cap06/08-event-scheduler.cpp
// Solved exercise: Event Scheduler with Conflicts
//
// Naive layer: O(n) per query.
// Algorithmic layer: Fenwick (counting) + set (conflict detection) -- O(log n).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o event_scheduler 08-event-scheduler.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feevent_scheduler.exe 08-event-scheduler.cpp

#include <cassert>
#include <climits>
#include <print>
#include <set>
#include <vector>

using i64 = long long;

struct Event { int start, end, id; };

// ============================================================
// Layer 1 -- Naive: O(n) per query.
// ============================================================
bool naive_has_conflict(const std::vector<Event>& scheduled, const Event& e) {
    for (const auto& s : scheduled)
        if (s.start < e.end && e.start < s.end) return true;
    return false;
}

int naive_active_at(const std::vector<Event>& scheduled, int t) {
    int cnt = 0;
    for (const auto& s : scheduled)
        if (s.start <= t && t < s.end) ++cnt;
    return cnt;
}

// ============================================================
// Simple Fenwick tree for active-event counting.
// ============================================================
struct Fenwick {
    int n;
    std::vector<int> bit;
    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void update(int i, int v) { for (++i; i <= n; i += i&-i) bit[i] += v; }
    int  query(int i)  const  { int s=0; for (++i; i>0; i -= i&-i) s+=bit[i]; return s; }
};

// ============================================================
// Layer 2 -- O(log n) per operation.
// std::set of (start, end) for overlap detection.
// Difference-style Fenwick for point active counts.
// ============================================================
struct EventScheduler {
    std::set<std::pair<int,int>> active;  // (start, end)
    Fenwick fw;

    explicit EventScheduler(int max_time) : fw(max_time + 1) {}

    bool has_conflict(const Event& e) const {
        // First event with start >= e.start.
        auto it = active.lower_bound(std::make_pair(e.start, INT_MIN));
        if (it != active.end() && it->first < e.end) return true;
        // Previous event: does it end after e.start?
        if (it != active.begin()) {
            --it;
            if (it->second > e.start) return true;
        }
        return false;
    }

    int active_at(int t) const { return fw.query(t); }

    void insert(const Event& e) {
        active.insert(std::make_pair(e.start, e.end));
        fw.update(e.start, 1);
        fw.update(e.end, -1);
    }
};

int main() {
    EventScheduler sched(20);
    std::vector<Event> log_events;

    Event e1 = {2, 8, 1};
    Event e2 = {5, 12, 2};
    Event e3 = {10, 15, 3};
    Event e4 = {1, 3, 4};

    assert(!sched.has_conflict(e1));
    sched.insert(e1);
    log_events.push_back(e1);

    assert(sched.has_conflict(e2));
    assert(naive_has_conflict(log_events, e2));
    assert(sched.active_at(6) == 1);
    assert(naive_active_at(log_events, 6) == 1);

    assert(!sched.has_conflict(e3));
    sched.insert(e3);
    log_events.push_back(e3);

    assert(sched.active_at(11) == 1);
    assert(sched.has_conflict(e4));

    std::print("conflict between e2 and e1: {}\n", sched.has_conflict(e2) ? "yes" : "no");
    std::print("active at t=6: {}\n", sched.active_at(6));
    std::print("active at t=11: {}\n", sched.active_at(11));
    std::print("conflict between e4 and schedule: {}\n", sched.has_conflict(e4) ? "yes" : "no");

    std::print("event-scheduler exercise: all assertions passed\n");
    return 0;
}
