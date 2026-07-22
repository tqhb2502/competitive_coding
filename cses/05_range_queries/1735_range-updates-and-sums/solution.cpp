#include <algorithm>
#include <iostream>
#include <vector>

using i64 = long long;

class SegmentTree {
public:
    explicit SegmentTree(const std::vector<i64>& values)
        : n_(static_cast<int>(values.size())),
          sum_(static_cast<std::size_t>(4 * n_), 0),
          lazy_add_(static_cast<std::size_t>(4 * n_), 0),
          lazy_assign_(static_cast<std::size_t>(4 * n_), 0),
          has_assign_(static_cast<std::size_t>(4 * n_), false) {
        build(1, 0, n_ - 1, values);
    }

    void add(int left, int right, i64 value) {
        add(1, 0, n_ - 1, left, right, value);
    }

    void assign(int left, int right, i64 value) {
        assign(1, 0, n_ - 1, left, right, value);
    }

    [[nodiscard]] i64 query(int left, int right) {
        return query(1, 0, n_ - 1, left, right);
    }

private:
    int n_;
    std::vector<i64> sum_;
    std::vector<i64> lazy_add_;
    std::vector<i64> lazy_assign_;
    std::vector<bool> has_assign_;

    void build(int node, int lo, int hi, const std::vector<i64>& values) {
        if (lo == hi) {
            sum_[static_cast<std::size_t>(node)] = values[static_cast<std::size_t>(lo)];
            return;
        }
        const int mid = lo + (hi - lo) / 2;
        build(node * 2, lo, mid, values);
        build(node * 2 + 1, mid + 1, hi, values);
        pull(node);
    }

    void pull(int node) {
        sum_[static_cast<std::size_t>(node)] =
            sum_[static_cast<std::size_t>(node * 2)] +
            sum_[static_cast<std::size_t>(node * 2 + 1)];
    }

    void apply_assign(int node, int length, i64 value) {
        const auto index = static_cast<std::size_t>(node);
        sum_[index] = value * static_cast<i64>(length);
        lazy_assign_[index] = value;
        lazy_add_[index] = 0;
        has_assign_[index] = true;
    }

    void apply_add(int node, int length, i64 value) {
        const auto index = static_cast<std::size_t>(node);
        sum_[index] += value * static_cast<i64>(length);
        lazy_add_[index] += value;
    }

    void push(int node, int lo, int hi) {
        if (lo == hi) {
            return;
        }
        const int mid = lo + (hi - lo) / 2;
        const int left_length = mid - lo + 1;
        const int right_length = hi - mid;
        const auto index = static_cast<std::size_t>(node);

        if (has_assign_[index]) {
            apply_assign(node * 2, left_length, lazy_assign_[index]);
            apply_assign(node * 2 + 1, right_length, lazy_assign_[index]);
            has_assign_[index] = false;
        }
        if (lazy_add_[index] != 0) {
            apply_add(node * 2, left_length, lazy_add_[index]);
            apply_add(node * 2 + 1, right_length, lazy_add_[index]);
            lazy_add_[index] = 0;
        }
    }

    void add(int node, int lo, int hi, int query_left, int query_right, i64 value) {
        if (query_left <= lo && hi <= query_right) {
            apply_add(node, hi - lo + 1, value);
            return;
        }
        push(node, lo, hi);
        const int mid = lo + (hi - lo) / 2;
        if (query_left <= mid) {
            add(node * 2, lo, mid, query_left, query_right, value);
        }
        if (query_right > mid) {
            add(node * 2 + 1, mid + 1, hi, query_left, query_right, value);
        }
        pull(node);
    }

    void assign(int node, int lo, int hi, int query_left, int query_right, i64 value) {
        if (query_left <= lo && hi <= query_right) {
            apply_assign(node, hi - lo + 1, value);
            return;
        }
        push(node, lo, hi);
        const int mid = lo + (hi - lo) / 2;
        if (query_left <= mid) {
            assign(node * 2, lo, mid, query_left, query_right, value);
        }
        if (query_right > mid) {
            assign(node * 2 + 1, mid + 1, hi, query_left, query_right, value);
        }
        pull(node);
    }

    i64 query(int node, int lo, int hi, int query_left, int query_right) {
        if (query_left <= lo && hi <= query_right) {
            return sum_[static_cast<std::size_t>(node)];
        }
        push(node, lo, hi);
        const int mid = lo + (hi - lo) / 2;
        i64 answer = 0;
        if (query_left <= mid) {
            answer += query(node * 2, lo, mid, query_left, query_right);
        }
        if (query_right > mid) {
            answer += query(node * 2 + 1, mid + 1, hi, query_left, query_right);
        }
        return answer;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;
    std::vector<i64> values(static_cast<std::size_t>(n));
    for (i64& value : values) {
        std::cin >> value;
    }

    SegmentTree tree(values);
    while (q-- > 0) {
        int type = 0;
        int left = 0;
        int right = 0;
        std::cin >> type >> left >> right;
        --left;
        --right;
        if (type == 3) {
            std::cout << tree.query(left, right) << '\n';
        } else {
            i64 value = 0;
            std::cin >> value;
            if (type == 1) {
                tree.add(left, right, value);
            } else {
                tree.assign(left, right, value);
            }
        }
    }
}
