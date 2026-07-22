#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<string> original(n);
    for (string& row : original) {
        cin >> row;
    }

    const array<char, 4> colors = {'A', 'B', 'C', 'D'};
    vector<string> answer(n, string(m, '?'));
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < m; ++col) {
            for (const char color : colors) {
                const bool sameAsOriginal = color == original[row][col];
                const bool sameAsAbove = row > 0 && color == answer[row - 1][col];
                const bool sameAsLeft = col > 0 && color == answer[row][col - 1];
                if (!sameAsOriginal && !sameAsAbove && !sameAsLeft) {
                    answer[row][col] = color;
                    break;
                }
            }
        }
    }

    for (const string& row : answer) {
        cout << row << '\n';
    }

    return 0;
}
