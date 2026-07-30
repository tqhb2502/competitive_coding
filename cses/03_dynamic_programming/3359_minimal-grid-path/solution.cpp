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

    // Ký tự đầu tiên bắt buộc là grid[0][0] (đường chéo d = 0 chỉ có ô (0,0)).
    string answer;
    answer.reserve(2 * n - 1);
    answer.push_back(grid[0][0]);

    // frontier[r] = true nếu hàng r nằm trong tập ô đạt tiền tố tối ưu chung
    // trên đường chéo hiện tại. Base case: frontier ban đầu = { hàng 0 }.
    vector<char> frontier(n, false);
    vector<char> nextFrontier(n, false);
    frontier[0] = true;

    // Duyệt tham lam từng đường chéo phụ d (các ô cùng r + c = d).
    for (int diagonal = 1; diagonal <= 2 * n - 2; ++diagonal) {
        // Khoảng hàng hợp lệ trên đường chéo d.
        const int lowestRow = max(0, diagonal - (n - 1));
        const int highestRow = min(n - 1, diagonal);
        char smallest = static_cast<char>('Z' + 1);

        // Tìm ký tự nhỏ nhất trong các ô ứng viên: hàng r là ứng viên nếu tới
        // được bằng đi phải (frontier[r]) hoặc đi xuống (frontier[r-1]).
        for (int row = lowestRow; row <= highestRow; ++row) {
            const bool reachable = frontier[row] ||
                                   (row > 0 && frontier[row - 1]);
            if (reachable) {
                const int column = diagonal - row;
                smallest = min(smallest, grid[row][column]);
            }
        }

        // frontier mới = các ô ứng viên có ký tự đúng bằng ký tự nhỏ nhất.
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
