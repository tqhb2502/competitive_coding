#include <algorithm>
#include <iostream>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(const int size) : size_(size), tree_(size + 1, 0) {}

    void add(int index, const int value) {
        while (index <= size_) {
            tree_[index] += value;
            index += index & -index;
        }
    }

    int prefix_sum(int index) const {
        int result = 0;
        while (index > 0) {
            result += tree_[index];
            index -= index & -index;
        }
        return result;
    }

private:
    int size_;
    std::vector<int> tree_;
};

struct Event {
    int value_rank;
    int sign;
    int query_index;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> values(n + 1);
    std::vector<int> sorted_values;
    sorted_values.reserve(n);
    for (int index = 1; index <= n; ++index) {
        std::cin >> values[index];
        sorted_values.push_back(values[index]);
    }
    std::sort(sorted_values.begin(), sorted_values.end());
    sorted_values.erase(
        std::unique(sorted_values.begin(), sorted_values.end()),
        sorted_values.end()
    );

    std::vector<std::vector<Event>> events(n + 1);
    std::vector<int> answers(q, 0);

    const auto rank_at_most = [&sorted_values](const int threshold) {
        return static_cast<int>(
            std::upper_bound(
                sorted_values.begin(), sorted_values.end(), threshold
            ) - sorted_values.begin()
        );
    };

    for (int query_index = 0; query_index < q; ++query_index) {
        int first, last, minimum_value, maximum_value;
        std::cin >> first >> last >> minimum_value >> maximum_value;

        const int upper_rank = rank_at_most(maximum_value);
        const int lower_rank = rank_at_most(minimum_value - 1);

        if (upper_rank > 0) {
            events[last].push_back({upper_rank, 1, query_index});
        }
        if (lower_rank > 0) {
            events[last].push_back({lower_rank, -1, query_index});
        }
        if (first > 1) {
            if (upper_rank > 0) {
                events[first - 1].push_back({upper_rank, -1, query_index});
            }
            if (lower_rank > 0) {
                events[first - 1].push_back({lower_rank, 1, query_index});
            }
        }
    }

    FenwickTree frequencies(static_cast<int>(sorted_values.size()));
    for (int index = 1; index <= n; ++index) {
        const int rank = static_cast<int>(
            std::lower_bound(
                sorted_values.begin(), sorted_values.end(), values[index]
            ) - sorted_values.begin()
        ) + 1;
        frequencies.add(rank, 1);

        for (const Event& event : events[index]) {
            answers[event.query_index] +=
                event.sign * frequencies.prefix_sum(event.value_rank);
        }
    }

    for (const int answer : answers) {
        std::cout << answer << '\n';
    }
    return 0;
}
