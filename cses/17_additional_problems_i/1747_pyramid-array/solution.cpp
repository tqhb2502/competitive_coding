#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

class FenwickTree {
public:
    explicit FenwickTree(int size) : tree(size + 1, 0) {}

    void add(int index, int value) {
        const int size = static_cast<int>(tree.size());
        for (int i = index; i < size; i += i & -i) {
            tree[i] += value;
        }
    }

    int prefixSum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree[i];
        }
        return result;
    }

private:
    vector<int> tree;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<long long, int>> elements;
    elements.reserve(n);
    for (int position = 1; position <= n; ++position) {
        long long value;
        cin >> value;
        elements.emplace_back(value, position);
    }
    sort(elements.begin(), elements.end());

    FenwickTree active(n);
    for (int position = 1; position <= n; ++position) {
        active.add(position, 1);
    }

    long long answer = 0;
    int remaining = n;
    for (const auto &[value, position] : elements) {
        static_cast<void>(value);
        const int left = active.prefixSum(position - 1);
        const int right = remaining - 1 - left;
        answer += min(left, right);
        active.add(position, -1);
        --remaining;
    }

    cout << answer << '\n';
}
