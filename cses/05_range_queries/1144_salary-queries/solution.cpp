// Salary Queries - CSES 1144
// Offline coordinate compression and a Fenwick tree of salary frequencies.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

struct Query {
    char type;
    int a;
    int b;
};

class FenwickTree {
public:
    explicit FenwickTree(std::size_t size) : tree_(size + 1, 0) {}

    void add(std::size_t position, int delta) {
        const std::size_t size = tree_.size() - 1;
        for (std::size_t i = position; i <= size; i += i & -i) {
            tree_[i] += delta;
        }
    }

    [[nodiscard]] int prefix_sum(std::size_t position) const {
        int result = 0;
        for (std::size_t i = position; i > 0; i -= i & -i) {
            result += tree_[i];
        }
        return result;
    }

private:
    std::vector<int> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    std::vector<int> salary(n);
    std::vector<int> values;
    values.reserve(n + q);
    for (int& value : salary) {
        std::cin >> value;
        values.push_back(value);
    }

    std::vector<Query> queries;
    queries.reserve(q);
    for (std::size_t i = 0; i < q; ++i) {
        Query query{};
        std::cin >> query.type >> query.a >> query.b;
        queries.push_back(query);
        if (query.type == '!') {
            values.push_back(query.b);
        }
    }

    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    const auto compressed_position = [&values](int value) {
        return static_cast<std::size_t>(
                   std::lower_bound(values.begin(), values.end(), value) - values.begin()) +
               1;
    };

    FenwickTree frequencies(values.size());
    std::vector<std::size_t> current_position(n);
    for (std::size_t i = 0; i < n; ++i) {
        current_position[i] = compressed_position(salary[i]);
        frequencies.add(current_position[i], 1);
    }

    for (const Query& query : queries) {
        if (query.type == '!') {
            const std::size_t employee = static_cast<std::size_t>(query.a - 1);
            frequencies.add(current_position[employee], -1);
            current_position[employee] = compressed_position(query.b);
            frequencies.add(current_position[employee], 1);
        } else {
            const std::size_t below_lower = static_cast<std::size_t>(
                std::lower_bound(values.begin(), values.end(), query.a) - values.begin());
            const std::size_t at_most_upper = static_cast<std::size_t>(
                std::upper_bound(values.begin(), values.end(), query.b) - values.begin());
            std::cout << frequencies.prefix_sum(at_most_upper) -
                             frequencies.prefix_sum(below_lower)
                      << '\n';
        }
    }
    return 0;
}
