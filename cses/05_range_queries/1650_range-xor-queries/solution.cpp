#include <iostream>
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

    vector<int> prefix_xor(to_index(n + 1), 0);
    for (int i = 1; i <= n; ++i) {
        int value;
        cin >> value;
        prefix_xor[to_index(i)] = prefix_xor[to_index(i - 1)] ^ value;
    }

    while (q--) {
        int left, right;
        cin >> left >> right;
        cout << (prefix_xor[to_index(right)] ^ prefix_xor[to_index(left - 1)])
             << '\n';
    }

    return 0;
}
