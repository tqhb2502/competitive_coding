#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Tìm đường tăng cho hàng row: thử ghép nó với một cột có đồng xu.
// Nếu cột trống hoặc hàng đang giữ cột đó tự đẩy sang chỗ khác được thì ghép.
bool augment(int row, const vector<string>& grid, vector<int>& columnMatch,
             vector<char>& seenColumn) {
    const int n = static_cast<int>(grid.size());
    for (int column = 0; column < n; ++column) {
        if (grid[row][column] != 'o' || seenColumn[column]) {
            continue;
        }
        seenColumn[column] = true;
        if (columnMatch[column] == -1 ||
            augment(columnMatch[column], grid, columnMatch, seenColumn)) {
            columnMatch[column] = row;
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> grid(n);
    for (string& row : grid) {
        cin >> row;
    }

    // Bước 1: xây matching cực đại giữa hàng và cột (mỗi ô 'o' là một cạnh).
    vector<int> columnMatch(n, -1);
    for (int row = 0; row < n; ++row) {
        vector<char> seenColumn(n, false);
        augment(row, grid, columnMatch, seenColumn);
    }

    // Ánh xạ ngược: rowMatch[row] là cột đang ghép với hàng đó.
    vector<int> rowMatch(n, -1);
    for (int column = 0; column < n; ++column) {
        if (columnMatch[column] != -1) {
            rowMatch[columnMatch[column]] = column;
        }
    }

    // Bước 2: BFS theo đường xen kẽ, xuất phát từ các hàng CHƯA được ghép.
    // side == 0: đỉnh là hàng; side == 1: đỉnh là cột.
    vector<char> seenRow(n, false);
    vector<char> seenColumn(n, false);
    queue<pair<int, int>> searchQueue;
    for (int row = 0; row < n; ++row) {
        if (rowMatch[row] == -1) {
            seenRow[row] = true;
            searchQueue.push({0, row});
        }
    }

    while (!searchQueue.empty()) {
        const auto [side, vertex] = searchQueue.front();
        searchQueue.pop();
        if (side == 0) {
            // Từ hàng đi sang cột qua cạnh KHÔNG thuộc matching.
            const int row = vertex;
            for (int column = 0; column < n; ++column) {
                if (grid[row][column] == 'o' && rowMatch[row] != column &&
                    !seenColumn[column]) {
                    seenColumn[column] = true;
                    searchQueue.push({1, column});
                }
            }
        } else {
            // Từ cột quay về hàng đang ghép với nó (cạnh thuộc matching).
            const int column = vertex;
            const int row = columnMatch[column];
            if (row != -1 && !seenRow[row]) {
                seenRow[row] = true;
                searchQueue.push({0, row});
            }
        }
    }

    // Bước 3: vertex cover König = (các hàng KHÔNG thăm) hợp (các cột ĐÃ thăm).
    vector<pair<int, int>> answer;
    for (int row = 0; row < n; ++row) {
        if (!seenRow[row]) {
            answer.push_back({1, row + 1});
        }
    }
    for (int column = 0; column < n; ++column) {
        if (seenColumn[column]) {
            answer.push_back({2, column + 1});
        }
    }

    // In số thao tác rồi liệt kê từng thao tác (loại và chỉ số 1-based).
    cout << answer.size() << '\n';
    for (const auto [type, index] : answer) {
        cout << type << ' ' << index << '\n';
    }
}
