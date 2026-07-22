#include <algorithm>
#include <iostream>
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
    vector<int> permutation(n);
    for (int &value : permutation) {
        cin >> value;
    }

    FenwickTree seen(n);
    long long inversions = 0;
    for (int index = 0; index < n; ++index) {
        inversions += index - seen.prefixSum(permutation[index]);
        seen.add(permutation[index], 1);
    }

    vector<bool> visited(n + 1, false);
    int cycles = 0;
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }
        ++cycles;
        int current = start;
        while (!visited[current]) {
            visited[current] = true;
            current = permutation[current - 1];
        }
    }
    const int arbitrarySwaps = n - cycles;

    vector<int> tails;
    tails.reserve(n);
    for (int value : permutation) {
        const auto place = lower_bound(tails.begin(), tails.end(), value);
        if (place == tails.end()) {
            tails.push_back(value);
        } else {
            *place = value;
        }
    }
    const int arbitraryMoves = n - static_cast<int>(tails.size());

    int expected = n;
    for (int index = n - 1; index >= 0; --index) {
        if (permutation[index] == expected) {
            --expected;
        }
    }
    const int movesToFront = expected;

    cout << inversions << ' ' << arbitrarySwaps << ' ' << arbitraryMoves << ' '
         << movesToFront << '\n';
}
