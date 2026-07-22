#include <iostream>
#include <string>
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

    vector<vector<int>> prefix(to_index(n + 1), vector<int>(to_index(n + 1), 0));
    for (int row = 1; row <= n; ++row) {
        string cells;
        cin >> cells;
        int row_sum = 0;
        for (int column = 1; column <= n; ++column) {
            row_sum += (cells[to_index(column - 1)] == '*');
            prefix[to_index(row)][to_index(column)] =
                prefix[to_index(row - 1)][to_index(column)] + row_sum;
        }
    }

    while (q--) {
        int y1, x1, y2, x2;
        cin >> y1 >> x1 >> y2 >> x2;
        const int answer = prefix[to_index(y2)][to_index(x2)]
                         - prefix[to_index(y1 - 1)][to_index(x2)]
                         - prefix[to_index(y2)][to_index(x1 - 1)]
                         + prefix[to_index(y1 - 1)][to_index(x1 - 1)];
        cout << answer << '\n';
    }

    return 0;
}
