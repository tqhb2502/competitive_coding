#include <algorithm>
#include <iostream>
#include <vector>

struct Query {
    int type;
    int first;
    int second;
};

class FenwickTree {
public:
    explicit FenwickTree(const int size)
        : size_(size), tree_(size + 1, 0) {
        highest_power_ = 1;
        while (highest_power_ <= size_ / 2) {
            highest_power_ *= 2;
        }
    }

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

    int select(const int order) const {
        int index = 0;
        int remaining = order;
        for (int step = highest_power_; step > 0; step /= 2) {
            const int next = index + step;
            if (next <= size_ && tree_[next] < remaining) {
                index = next;
                remaining -= tree_[next];
            }
        }
        return index + 1;
    }

private:
    int size_;
    int highest_power_;
    std::vector<int> tree_;
};

class MaximumSegmentTree {
public:
    explicit MaximumSegmentTree(const std::vector<int>& values) {
        const int count = static_cast<int>(values.size()) - 1;
        size_ = 1;
        while (size_ < count) {
            size_ *= 2;
        }
        tree_.assign(2 * size_, 0);
        for (int position = 1; position <= count; ++position) {
            tree_[size_ + position - 1] = values[position];
        }
        for (int index = size_ - 1; index > 0; --index) {
            tree_[index] = std::max(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    void assign(const int position, const int value) {
        int index = size_ + position - 1;
        tree_[index] = value;
        for (index /= 2; index > 0; index /= 2) {
            tree_[index] = std::max(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    int query(int left, const int right) const {
        int result = 0;
        left += size_ - 1;
        int end = right + size_;
        while (left < end) {
            if ((left & 1) != 0) {
                result = std::max(result, tree_[left++]);
            }
            if ((end & 1) != 0) {
                result = std::max(result, tree_[--end]);
            }
            left /= 2;
            end /= 2;
        }
        return result;
    }

private:
    int size_ = 1;
    std::vector<int> tree_;
};

long long pair_key(const int value_rank, const int position, const int stride) {
    return static_cast<long long>(value_rank) * stride + position;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> values(n + 1);
    std::vector<int> all_values;
    all_values.reserve(n + q);
    for (int position = 1; position <= n; ++position) {
        std::cin >> values[position];
        all_values.push_back(values[position]);
    }

    std::vector<Query> queries;
    queries.reserve(q);
    for (int query_index = 0; query_index < q; ++query_index) {
        Query query{};
        std::cin >> query.type >> query.first >> query.second;
        queries.push_back(query);
        if (query.type == 1) {
            all_values.push_back(query.second);
        }
    }

    std::sort(all_values.begin(), all_values.end());
    all_values.erase(
        std::unique(all_values.begin(), all_values.end()), all_values.end()
    );
    const auto value_rank = [&all_values](const int value) {
        return static_cast<int>(
            std::lower_bound(all_values.begin(), all_values.end(), value) -
            all_values.begin()
        );
    };

    const int stride = n + 1;
    std::vector<long long> possible_pairs;
    possible_pairs.reserve(n + q);
    for (int position = 1; position <= n; ++position) {
        possible_pairs.push_back(
            pair_key(value_rank(values[position]), position, stride)
        );
    }
    for (const Query& query : queries) {
        if (query.type == 1) {
            possible_pairs.push_back(
                pair_key(value_rank(query.second), query.first, stride)
            );
        }
    }
    std::sort(possible_pairs.begin(), possible_pairs.end());
    possible_pairs.erase(
        std::unique(possible_pairs.begin(), possible_pairs.end()),
        possible_pairs.end()
    );

    const int pair_count = static_cast<int>(possible_pairs.size());
    std::vector<int> pair_position(pair_count + 1, 0);
    std::vector<int> pair_value_rank(pair_count + 1, 0);
    for (int index = 1; index <= pair_count; ++index) {
        const long long key = possible_pairs[index - 1];
        pair_position[index] = static_cast<int>(key % stride);
        pair_value_rank[index] = static_cast<int>(key / stride);
    }

    const auto compressed_pair = [&possible_pairs](const long long key) {
        return static_cast<int>(
            std::lower_bound(possible_pairs.begin(), possible_pairs.end(), key) -
            possible_pairs.begin()
        ) + 1;
    };

    std::vector<int> current_pair(n + 1, 0);
    FenwickTree active_pairs(pair_count);
    for (int position = 1; position <= n; ++position) {
        current_pair[position] = compressed_pair(
            pair_key(value_rank(values[position]), position, stride)
        );
        active_pairs.add(current_pair[position], 1);
    }

    std::vector<int> previous(n + 1, 0);
    std::vector<int> last_position(all_values.size(), 0);
    for (int position = 1; position <= n; ++position) {
        const int rank = value_rank(values[position]);
        previous[position] = last_position[rank];
        last_position[rank] = position;
    }
    MaximumSegmentTree maximum_previous(previous);

    const auto same_value_predecessor = [
        &active_pairs, &pair_position, &pair_value_rank
    ](const int pair_index, const int rank) {
        const int earlier_count = active_pairs.prefix_sum(pair_index - 1);
        if (earlier_count == 0) {
            return 0;
        }
        const int predecessor = active_pairs.select(earlier_count);
        return pair_value_rank[predecessor] == rank
                   ? pair_position[predecessor]
                   : 0;
    };

    const auto same_value_successor = [
        &active_pairs, &pair_position, &pair_value_rank
    ](const int pair_index, const int rank, const int active_count) {
        const int through_current = active_pairs.prefix_sum(pair_index);
        if (through_current >= active_count) {
            return 0;
        }
        const int successor = active_pairs.select(through_current + 1);
        return pair_value_rank[successor] == rank
                   ? pair_position[successor]
                   : 0;
    };

    for (const Query& query : queries) {
        if (query.type == 2) {
            std::cout << (
                maximum_previous.query(query.first, query.second) < query.first
                    ? "YES\n"
                    : "NO\n"
            );
            continue;
        }

        const int position = query.first;
        const int old_pair = current_pair[position];
        const int new_pair = compressed_pair(
            pair_key(value_rank(query.second), position, stride)
        );
        if (old_pair == new_pair) {
            continue;
        }

        const int old_rank = pair_value_rank[old_pair];
        active_pairs.add(old_pair, -1);
        const int old_predecessor =
            same_value_predecessor(old_pair, old_rank);
        const int old_successor =
            same_value_successor(old_pair, old_rank, n - 1);
        if (old_successor != 0) {
            maximum_previous.assign(old_successor, old_predecessor);
        }

        active_pairs.add(new_pair, 1);
        const int new_rank = pair_value_rank[new_pair];
        const int new_predecessor =
            same_value_predecessor(new_pair, new_rank);
        const int new_successor =
            same_value_successor(new_pair, new_rank, n);
        maximum_previous.assign(position, new_predecessor);
        if (new_successor != 0) {
            maximum_previous.assign(new_successor, position);
        }
        current_pair[position] = new_pair;
    }

    return 0;
}
