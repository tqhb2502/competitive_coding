#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class FenwickTree {
public:
    explicit FenwickTree(int size) : tree_(size + 1, 0) {}

    void add(int index, int value) {
        while (index < static_cast<int>(tree_.size())) {
            tree_[index] += value;
            if (tree_[index] >= MOD) {
                tree_[index] -= MOD;
            }
            index += index & -index;
        }
    }

    int prefix_sum(int index) const {
        long long result = 0;
        while (index > 0) {
            result += tree_[index];
            index -= index & -index;
        }
        return static_cast<int>(result % MOD);
    }

private:
    static constexpr int MOD = 1'000'000'007;
    vector<int> tree_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n;
    cin >> n;
    vector<long long> values(n);
    for (long long& value : values) {
        cin >> value;
    }

    vector<long long> compressed = values;
    sort(compressed.begin(), compressed.end());
    compressed.erase(unique(compressed.begin(), compressed.end()), compressed.end());

    FenwickTree fenwick(static_cast<int>(compressed.size()));
    long long answer = 0;
    for (const long long value : values) {
        const int rank = static_cast<int>(
                             lower_bound(compressed.begin(), compressed.end(), value) - compressed.begin()) +
                         1;

        // Query rank - 1 so equal values are never appended to each other.
        const int ending_here = (fenwick.prefix_sum(rank - 1) + 1LL) % MOD;
        answer += ending_here;
        if (answer >= MOD) {
            answer -= MOD;
        }
        fenwick.add(rank, ending_here);
    }

    cout << answer << '\n';
    return 0;
}
