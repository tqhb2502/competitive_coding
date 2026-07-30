#include <bits/stdc++.h>
using namespace std;

#include "cp_library/02_data_structures/dsu.hpp"
#include "cp_library/02_data_structures/fenwick.hpp"
#include "cp_library/02_data_structures/lazy-segment-tree.hpp"
#include "cp_library/02_data_structures/li-chao-tree.hpp"

namespace {

mt19937_64 rng(0xC5E5A91ULL);

[[noreturn]] void fail(const string& message) {
    cerr << "linkage API regression failure: " << message << '\n';
    exit(1);
}

void require(bool condition, const string& message) {
    if (!condition) fail(message);
}

long long randomLong(long long low, long long high) {
    return low + (long long)(rng() % (uint64_t)(high - low + 1));
}

void testLazyAddAssignSum() {
    for (int iteration = 0; iteration < 150; ++iteration) {
        int n = (int)randomLong(1, 35);
        vector<long long> brute(n);
        for (long long& value : brute) value = randomLong(-30, 30);
        LazySeg tree(brute);

        for (int step = 0; step < 350; ++step) {
            int left = (int)randomLong(0, n);
            int right = (int)randomLong(left, n);
            int operation = (int)randomLong(0, 2);
            if (operation == 0) {
                long long delta = randomLong(-20, 20);
                tree.update(left, right, delta);
                for (int i = left; i < right; ++i) brute[i] += delta;
            } else if (operation == 1) {
                long long value = randomLong(-30, 30);
                tree.assign(left, right, value);
                for (int i = left; i < right; ++i) brute[i] = value;
            } else {
                long long expected =
                    accumulate(brute.begin() + left, brute.begin() + right, 0LL);
                require(tree.query(left, right) == expected,
                        "LazySeg add/assign/sum");
            }
        }
    }

    LazySeg empty({});
    empty.update(0, 0, 3);
    empty.assign(0, 0, 7);
    require(empty.query(0, 0) == 0, "LazySeg empty");
}

void testRangeAddMinMax() {
    for (int iteration = 0; iteration < 150; ++iteration) {
        int n = (int)randomLong(1, 35);
        vector<long long> brute(n);
        for (long long& value : brute) value = randomLong(-50, 50);
        RangeAddMinMax tree(brute);

        for (int step = 0; step < 350; ++step) {
            if (randomLong(0, 1) == 0) {
                int left = (int)randomLong(0, n);
                int right = (int)randomLong(left, n);
                long long delta = randomLong(-20, 20);
                tree.rangeAdd(left, right, delta);
                for (int i = left; i < right; ++i) brute[i] += delta;
            } else {
                int left = (int)randomLong(0, n - 1);
                int right = (int)randomLong(left + 1, n);
                auto [minimum, maximum] =
                    minmax_element(brute.begin() + left, brute.begin() + right);
                require(tree.rangeMin(left, right) == *minimum,
                        "RangeAddMinMax minimum");
                require(tree.rangeMax(left, right) == *maximum,
                        "RangeAddMinMax maximum");
            }
            require(tree.allMin() == *min_element(brute.begin(), brute.end()),
                    "RangeAddMinMax allMin");
            require(tree.allMax() == *max_element(brute.begin(), brute.end()),
                    "RangeAddMinMax allMax");
        }
    }
}

void testCoveredLength() {
    const vector<long long> coordinates = {-10, -7, -1, 0, 4, 9, 15};
    CoveredLengthSegTree tree(coordinates);
    vector<int> count(coordinates.size() - 1, 0);
    vector<pair<int, int>> active;

    for (int step = 0; step < 3000; ++step) {
        bool remove = !active.empty() && randomLong(0, 2) == 0;
        int left, right, delta;
        if (remove) {
            int at = (int)randomLong(0, (int)active.size() - 1);
            tie(left, right) = active[at];
            active[at] = active.back();
            active.pop_back();
            delta = -1;
        } else {
            left = (int)randomLong(0, (int)coordinates.size() - 2);
            right = (int)randomLong(left + 1, (int)coordinates.size() - 1);
            active.push_back({left, right});
            delta = 1;
        }
        tree.addCoordinateRange(coordinates[left], coordinates[right], delta);
        for (int i = left; i < right; ++i) count[i] += delta;

        long long expected = 0;
        for (int i = 0; i + 1 < (int)coordinates.size(); ++i)
            if (count[i] > 0) expected += coordinates[i + 1] - coordinates[i];
        require(tree.coveredLength() == expected,
                "CoveredLengthSegTree union length");
    }

    // Một lớp phủ toàn miền đã nằm ở node cha; update con phải vẫn thấy lớp đó.
    // Cách lưu coverCount cục bộ không push từng gây false "count phủ âm" ở bước cuối.
    CoveredLengthSegTree mixed({0, 1, 2});
    mixed.addIndexRange(0, 2, 1);
    mixed.addIndexRange(0, 1, 1);
    mixed.addIndexRange(0, 1, -1);
    mixed.addIndexRange(0, 1, -1);
    require(mixed.coveredLength() == 1,
            "CoveredLengthSegTree arbitrary nonnegative range-add");
}

void testFenwickPrefixMax() {
    constexpr int n = 80;
    constexpr long long identity = -123456789012345LL;
    FenwickPrefixMax tree(n, identity);
    vector<long long> brute(n, identity);

    for (int step = 0; step < 5000; ++step) {
        if (randomLong(0, 2) != 0) {
            int index = (int)randomLong(0, n - 1);
            long long value = randomLong(-1000000, 1000000);
            tree.chmax(index, value);
            brute[index] = max(brute[index], value);
        } else {
            int right = (int)randomLong(0, n);
            long long expected = identity;
            for (int i = 0; i < right; ++i) expected = max(expected, brute[i]);
            require(tree.prefixMax(right) == expected,
                    "FenwickPrefixMax prefix");
        }
    }
}

void testParityDSU() {
    constexpr int n = 100;
    vector<int> hidden(n);
    for (int& value : hidden) value = (int)randomLong(0, 1);
    ParityDSU dsu(n);

    // Nối mỗi đỉnh vào một cây ngẫu nhiên bằng các ràng buộc nhất quán.
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent = (int)randomLong(0, vertex - 1);
        require(dsu.addConstraint(vertex, parent,
                                  hidden[vertex] ^ hidden[parent]),
                "ParityDSU consistent merge");
    }
    require(dsu.comp == 1, "ParityDSU component count");

    for (int step = 0; step < 5000; ++step) {
        int a = (int)randomLong(0, n - 1);
        int b = (int)randomLong(0, n - 1);
        auto difference = dsu.difference(a, b);
        require(difference && *difference == (hidden[a] ^ hidden[b]),
                "ParityDSU difference");
        require(dsu.addConstraint(a, b, hidden[a] ^ hidden[b]),
                "ParityDSU repeated consistent constraint");
    }
    require(!dsu.addConstraint(0, n - 1,
                               (hidden[0] ^ hidden[n - 1]) ^ 1),
            "ParityDSU contradiction");
}

struct RestrictedLine {
    long long slope, intercept, left, right;
};

optional<long long> bruteLiChao(const vector<RestrictedLine>& lines, long long x,
                                LiChao::Objective objective) {
    optional<LiChao::Wide> result;
    for (const auto& line : lines) {
        if (x < line.left || x > line.right) continue;
        LiChao::Wide value = (LiChao::Wide)line.slope * x + line.intercept;
        if (!result ||
            (objective == LiChao::Objective::Minimum ? value < *result
                                                     : value > *result))
            result = value;
    }
    if (!result) return nullopt;
    return (long long)*result;
}

void testLiChao() {
    constexpr long long low = -25, high = 25;
    for (LiChao::Objective objective :
         {LiChao::Objective::Minimum, LiChao::Objective::Maximum}) {
        LiChao tree(low, high, objective);
        vector<RestrictedLine> lines;
        require(!tree.queryIfAny(0), "LiChao empty query");

        for (int step = 0; step < 1200; ++step) {
            long long slope = randomLong(-30, 30);
            long long intercept = randomLong(-100, 100);
            long long left = low, right = high;
            if (randomLong(0, 1)) {
                left = randomLong(low, high);
                right = randomLong(left, high);
                tree.addSegment(slope, intercept, left, right);
            } else {
                tree.addLine(slope, intercept);
            }
            lines.push_back({slope, intercept, left, right});

            for (int sample = 0; sample < 5; ++sample) {
                long long x = randomLong(low, high);
                require(tree.queryIfAny(x) == bruteLiChao(lines, x, objective),
                        "LiChao randomized line/segment");
            }
        }
    }

    LiChao extreme(LLONG_MIN, LLONG_MAX);
    extreme.addLine(0, 7);
    require(extreme.query(LLONG_MIN) == 7 &&
            extreme.query(LLONG_MAX) == 7,
            "LiChao full signed domain");
    extreme.addSegment(0, -2, LLONG_MAX, LLONG_MAX);
    require(extreme.query(LLONG_MAX) == -2 &&
            extreme.query(LLONG_MAX - 1) == 7,
            "LiChao singleton segment at LLONG_MAX");

    LiChao overflow(2, 2);
    overflow.addLine(LLONG_MAX, LLONG_MAX);
    bool threw = false;
    try {
        (void)overflow.query(2);
    } catch (const overflow_error&) {
        threw = true;
    }
    require(threw, "LiChao checked long long conversion");
}

}  // namespace

int main() {
    testLazyAddAssignSum();
    testRangeAddMinMax();
    testCoveredLength();
    testFenwickPrefixMax();
    testParityDSU();
    testLiChao();
    cout << "linkage API regression tests passed\n";
    return 0;
}
