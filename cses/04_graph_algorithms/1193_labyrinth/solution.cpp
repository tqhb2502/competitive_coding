#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Đọc lưới và xác định chỉ số phẳng của ô xuất phát 'A' và ô đích 'B'.
    std::vector<std::string> grid(n);
    int start = -1;
    int target = -1;
    for (int row = 0; row < n; ++row) {
        std::cin >> grid[row];
        for (int column = 0; column < m; ++column) {
            if (grid[row][column] == 'A') {
                start = row * m + column;
            } else if (grid[row][column] == 'B') {
                target = row * m + column;
            }
        }
    }

    // previous_move[cell]: hướng đã dùng để đi vào ô đó, đồng thời đánh dấu đã thăm
    // (0 = chưa thăm, 'S' = ô xuất phát).
    std::vector<char> previous_move(n * m, 0);
    previous_move[start] = 'S';
    std::queue<int> queue;
    queue.push(start);

    constexpr int DR[4] = {-1, 1, 0, 0};
    constexpr int DC[4] = {0, 0, -1, 1};
    constexpr char MOVE[4] = {'U', 'D', 'L', 'R'};

    // BFS từ 'A': mọi cạnh trọng số 1 nên đường đi tìm được là ngắn nhất theo số bước.
    while (!queue.empty() && previous_move[target] == 0) {
        const int cell = queue.front();
        queue.pop();
        const int row = cell / m;
        const int column = cell % m;

        // Xét 4 ô kề cạnh; ô nào trong lưới, không phải tường và chưa thăm thì thăm.
        for (int direction = 0; direction < 4; ++direction) {
            const int nr = row + DR[direction];
            const int nc = column + DC[direction];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
                continue;
            }

            const int neighbor = nr * m + nc;
            if (grid[nr][nc] != '#' && previous_move[neighbor] == 0) {
                previous_move[neighbor] = MOVE[direction];
                queue.push(neighbor);
            }
        }
    }

    // Nếu 'B' chưa được thăm thì không tồn tại đường đi.
    if (previous_move[target] == 0) {
        std::cout << "NO\n";
        return 0;
    }

    // Truy vết ngược từ 'B' về 'A': đọc hướng đã lưu và lùi về ô liền trước.
    std::string path;
    for (int cell = target; cell != start;) {
        const char move = previous_move[cell];
        path.push_back(move);
        if (move == 'U') {
            cell += m;
        } else if (move == 'D') {
            cell -= m;
        } else if (move == 'L') {
            ++cell;
        } else {
            --cell;
        }
    }
    // Đảo ngược để có thứ tự đúng từ 'A' tới 'B'.
    std::reverse(path.begin(), path.end());

    std::cout << "YES\n" << path.size() << '\n' << path << '\n';
    return 0;
}
