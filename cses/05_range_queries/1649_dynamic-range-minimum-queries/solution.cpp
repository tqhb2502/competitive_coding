#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

static size_t to_index(int value) {
    return static_cast<size_t>(value);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    int size = 1;
    while (size < n) {
        size <<= 1;
    }

    const long long INF = numeric_limits<long long>::max();
    vector<long long> tree(to_index(2 * size), INF);
    for (int i = 0; i < n; ++i) {
        cin >> tree[to_index(size + i)];
    }
    for (int node = size - 1; node >= 1; --node) {
        tree[to_index(node)] = min(tree[to_index(2 * node)],
                                   tree[to_index(2 * node + 1)]);
    }

    while (q--) {
        int type, first;
        long long second;
        cin >> type >> first >> second;

        if (type == 1) {
            int node = size + first - 1;
            tree[to_index(node)] = second;
            for (node >>= 1; node >= 1; node >>= 1) {
                tree[to_index(node)] = min(tree[to_index(2 * node)],
                                           tree[to_index(2 * node + 1)]);
            }
        } else {
            int left = size + first - 1;
            int right = size + static_cast<int>(second);
            long long answer = INF;
            while (left < right) {
                if (left & 1) {
                    answer = min(answer, tree[to_index(left++)]);
                }
                if (right & 1) {
                    answer = min(answer, tree[to_index(--right)]);
                }
                left >>= 1;
                right >>= 1;
            }
            cout << answer << '\n';
        }
    }

    return 0;
}
