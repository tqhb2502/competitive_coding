#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> grid(n);
    for (string& row : grid) {
        cin >> row;
    }

    string answer;
    answer.reserve(2 * n - 1);
    answer.push_back(grid[0][0]);

    vector<char> frontier(n, false);
    vector<char> nextFrontier(n, false);
    frontier[0] = true;

    for (int diagonal = 1; diagonal <= 2 * n - 2; ++diagonal) {
        const int lowestRow = max(0, diagonal - (n - 1));
        const int highestRow = min(n - 1, diagonal);
        char smallest = static_cast<char>('Z' + 1);

        for (int row = lowestRow; row <= highestRow; ++row) {
            const bool reachable = frontier[row] ||
                                   (row > 0 && frontier[row - 1]);
            if (reachable) {
                const int column = diagonal - row;
                smallest = min(smallest, grid[row][column]);
            }
        }

        fill(nextFrontier.begin(), nextFrontier.end(), false);
        for (int row = lowestRow; row <= highestRow; ++row) {
            const bool reachable = frontier[row] ||
                                   (row > 0 && frontier[row - 1]);
            const int column = diagonal - row;
            if (reachable && grid[row][column] == smallest) {
                nextFrontier[row] = true;
            }
        }

        answer.push_back(smallest);
        frontier.swap(nextFrontier);
    }

    cout << answer << '\n';
    return 0;
}
