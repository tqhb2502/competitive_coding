#include <iostream>
#include <vector>

using namespace std;

static size_t to_index(int value) {
    return static_cast<size_t>(value);
}

class FenwickTree {
public:
    explicit FenwickTree(int size) : tree_(to_index(size + 1), 0) {}

    void add(int position, long long delta) {
        for (int i = position; i < static_cast<int>(tree_.size()); i += i & -i) {
            tree_[to_index(i)] += delta;
        }
    }

    long long prefix_sum(int position) const {
        long long result = 0;
        for (int i = position; i > 0; i -= i & -i) {
            result += tree_[to_index(i)];
        }
        return result;
    }

private:
    vector<long long> tree_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    FenwickTree differences(n);
    long long previous = 0;
    for (int i = 1; i <= n; ++i) {
        long long value;
        cin >> value;
        differences.add(i, value - previous);
        previous = value;
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int left, right;
            long long delta;
            cin >> left >> right >> delta;
            differences.add(left, delta);
            if (right < n) {
                differences.add(right + 1, -delta);
            }
        } else {
            int position;
            cin >> position;
            cout << differences.prefix_sum(position) << '\n';
        }
    }

    return 0;
}
