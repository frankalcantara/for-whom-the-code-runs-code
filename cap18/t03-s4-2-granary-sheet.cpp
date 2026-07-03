// Exercise T03.6 - The Granary Intake Sheet
// Session: T03.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 03 (contract validation in order; std::expected with a
//   structured reason; first-failure semantics)
// Tags: chapter-03
//
// MODEL: a sheet is valid when n complete records exist, every sack count
//   is positive, and no farmer repeats. The verdict is ACCEPTED with the
//   total, or REJECTED with the first reason in record order.
// MATH: "first reason found, checking each record in order" fixes the
//   priority by position, not by category: record i is fully judged
//   (present, well-formed, positive, fresh) before record i+1 is read.
//   Freshness needs the set of farmers already seen, which is membership
//   over a sparse domain of up to 1e9 identifiers: a hash set, not an
//   array of a billion flags.
// PROOF: loop invariant. Before reading record i, the first i-1 records
//   are individually valid, `seen` contains exactly their farmers, and
//   `total` is the sum of their sacks. Each check can therefore only fire
//   on the first offending record. If the loop completes, all n records
//   are valid and the invariant makes the total exact.
// COMPLEXITY: expected O(n) time, O(n) memory for the seen set.
// TYPES: totals reach 1e5 * 1e9 = 1e14: long long. Farmer ids fit in int
//   but are held as long long to keep the parse uniform; the sack count
//   must be read as a signed value, because rejecting "not positive"
//   requires representing the negative evidence first.
// ALTERNATIVES: reading all records before validating wastes nothing here
//   but breaks first-failure semantics if a later line is malformed while
//   an earlier count is bad; exceptions or exit() mid-parse would work and
//   hide the contract. Returning std::expected<long long, std::string_view>
//   from the validator keeps both outcomes in one visible type.
// EDGE CASES: stream ending mid-record (truncated); sack count zero or
//   negative (bad count); the duplicate appearing as the last record; a
//   minimal sheet of one record; ids that collide only as values, not as
//   text (1 vs 01 parse to the same farmer, which is the intended law).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o granary t03-s4-2-granary-sheet.cpp

#include <expected>
#include <iostream>
#include <print>
#include <string_view>
#include <unordered_set>

std::expected<long long, std::string_view> audit(std::istream& in) {
    int n = 0;
    if (!(in >> n)) return std::unexpected("truncated");

    std::unordered_set<long long> seen;
    seen.reserve(n);

    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long farmer = 0;
        long long sacks = 0;
        if (!(in >> farmer >> sacks)) return std::unexpected("truncated");
        if (sacks <= 0) return std::unexpected("bad count");
        if (!seen.insert(farmer).second)
            return std::unexpected("duplicate farmer");
        total += sacks;
    }
    return total;
}

int main() {
    auto verdict = audit(std::cin);
    if (verdict) std::print("ACCEPTED {}\n", *verdict);
    else         std::print("REJECTED {}\n", verdict.error());
    return 0;
}
