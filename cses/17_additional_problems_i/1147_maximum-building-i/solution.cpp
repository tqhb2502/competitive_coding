#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows, columns;
    cin >> rows >> columns;
    vector<int> height(columns + 1, 0);
    int answer = 0;

    for (int row = 0; row < rows; ++row) {
        string line;
        cin >> line;
        for (int column = 0; column < columns; ++column) {
            if (line[column] == '.') {
                ++height[column];
            } else {
                height[column] = 0;
            }
        }

        vector<int> increasingStack;
        increasingStack.reserve(columns + 1);
        for (int column = 0; column <= columns; ++column) {
            while (!increasingStack.empty() &&
                   height[increasingStack.back()] > height[column]) {
                const int currentHeight = height[increasingStack.back()];
                increasingStack.pop_back();
                const int left = increasingStack.empty() ? 0 : increasingStack.back() + 1;
                answer = max(answer, currentHeight * (column - left));
            }
            increasingStack.push_back(column);
        }
    }

    cout << answer << '\n';
}
