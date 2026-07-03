// Exercise T15.2 - The Billboard Plates
// Session: T15.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 15 (custom ordering by concatenation)
// Tags: chapter-15, greedy
//
// MODEL: arrange strings of digits so their concatenation is
//   lexicographically, and therefore numerically for fixed total
//   length, maximum.
// MATH: for two plates a and b, the only local question is whether
//   a should precede b. If ab > ba, any optimal row puts a before b;
//   otherwise putting b first is at least as good. Sorting by this
//   comparator gives a globally optimal order because every adjacent
//   inversion can be exchanged without decreasing the final string.
// PROOF: exchange argument. Consider any arrangement with adjacent
//   plates a, b such that ba > ab. Swapping them improves the whole
//   concatenation, since the prefix before them and suffix after them
//   are unchanged. Repeating this exchange reaches an order with no
//   bad adjacent pair, exactly the order produced by sorting with the
//   comparator. Such an order cannot be improved by adjacent swaps,
//   so it is optimal.
// COMPLEXITY: O(total_digits * log n) character comparisons in the
//   usual sort model; the comparator inspects at most |a| + |b|
//   characters and allocates no temporary concatenations. Memory is
//   O(total_digits).
// TYPES: the answer may have 2e5 digits, so it is a string, not an
//   integer. Indices fit size_t; no numeric conversion is performed.
// ALTERNATIVES: numeric sorting is wrong for pairs such as 3 and 30;
//   building a+b and b+a in every comparison is simpler but creates
//   avoidable allocation pressure at the stated input size.
// EDGE CASES: all plates are 0 (print one 0); equal comparator pairs
//   such as 12 and 1212; one plate; long common prefixes.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o plates t15-s2-1-billboard-plates.cpp

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool better_plate(const std::string& a, const std::string& b) {
    const std::size_t na = a.size();
    const std::size_t nb = b.size();
    for (std::size_t i = 0; i < na + nb; ++i) {
        const char ca = (i < na) ? a[i] : b[i - na];
        const char cb = (i < nb) ? b[i] : a[i - nb];
        if (ca != cb) return ca > cb;
    }
    return false;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<std::string> plates(n);
    for (std::string& plate : plates) std::cin >> plate;

    std::sort(plates.begin(), plates.end(), better_plate);

    if (!plates.empty() && plates.front() == "0") {
        std::cout << "0\n";
        return 0;
    }

    for (const std::string& plate : plates) std::cout << plate;
    std::cout << '\n';
    return 0;
}
