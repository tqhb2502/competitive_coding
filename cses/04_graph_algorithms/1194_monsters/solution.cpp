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

    const int size = n * m;
    const int infinity = size + 5;
    std::vector<std::string> grid(n);
    std::vector<int> monster_time(size, infinity);   // thời điểm sớm nhất quái vật tới ô
    std::queue<int> monster_queue;
    int start = -1;

    // Đọc lưới; đánh dấu nguồn multi-source (các ô 'M') và vị trí xuất phát 'A'
    for (int row = 0; row < n; ++row) {
        std::cin >> grid[row];
        for (int column = 0; column < m; ++column) {
            const int cell = row * m + column;
            if (grid[row][column] == 'M') {
                monster_time[cell] = 0;
                monster_queue.push(cell);
            } else if (grid[row][column] == 'A') {
                start = cell;
            }
        }
    }

    constexpr int DR[4] = {-1, 1, 0, 0};
    constexpr int DC[4] = {0, 0, -1, 1};
    constexpr char MOVE[4] = {'U', 'D', 'L', 'R'};

    // Lượt 1: multi-source BFS từ tất cả quái vật -> thời điểm sớm nhất mỗi ô bị chiếm
    while (!monster_queue.empty()) {
        const int cell = monster_queue.front();
        monster_queue.pop();
        const int row = cell / m;
        const int column = cell % m;

        for (int direction = 0; direction < 4; ++direction) {
            const int nr = row + DR[direction];
            const int nc = column + DC[direction];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m ||
                grid[nr][nc] == '#') {
                continue;
            }

            const int neighbor = nr * m + nc;
            if (monster_time[neighbor] == infinity) {
                monster_time[neighbor] = monster_time[cell] + 1;
                monster_queue.push(neighbor);
            }
        }
    }

    // Kiểm tra một ô có nằm trên biên lưới hay không (đích cần thoát ra)
    const auto is_boundary = [n, m](int cell) {
        const int row = cell / m;
        const int column = cell % m;
        return row == 0 || row == n - 1 || column == 0 || column == m - 1;
    };

    std::vector<int> player_time(size, infinity);
    std::vector<int> parent(size, -1);
    std::vector<char> previous_move(size, 0);   // ký tự di chuyển để tới ô này
    std::queue<int> player_queue;
    int exit_cell = -1;

    // Xuất phát chỉ hợp lệ khi người chơi tới trước quái vật (0 < monster_time[start])
    if (monster_time[start] > 0) {
        player_time[start] = 0;
        if (is_boundary(start)) {
            exit_cell = start;          // 'A' đã nằm sẵn trên biên
        } else {
            player_queue.push(start);
        }
    }

    // Lượt 2: BFS người chơi, chỉ đi vào ô tới được TRƯỚC HẲN mọi quái vật
    while (!player_queue.empty() && exit_cell == -1) {
        const int cell = player_queue.front();
        player_queue.pop();
        const int row = cell / m;
        const int column = cell % m;

        for (int direction = 0; direction < 4; ++direction) {
            const int nr = row + DR[direction];
            const int nc = column + DC[direction];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m ||
                grid[nr][nc] == '#') {
                continue;
            }

            const int neighbor = nr * m + nc;
            const int arrival = player_time[cell] + 1;
            // Bỏ qua nếu đã thăm, hoặc quái vật tới bằng/trước (arrival >= monster_time)
            if (player_time[neighbor] != infinity ||
                arrival >= monster_time[neighbor]) {
                continue;
            }

            player_time[neighbor] = arrival;
            parent[neighbor] = cell;
            previous_move[neighbor] = MOVE[direction];
            if (is_boundary(neighbor)) {
                exit_cell = neighbor;   // chạm biên an toàn -> dừng
                break;
            }
            player_queue.push(neighbor);
        }
    }

    if (exit_cell == -1) {
        std::cout << "NO\n";
        return 0;
    }

    // Truy vết cha để dựng lại đường đi rồi đảo ngược cho đúng chiều
    std::string path;
    for (int cell = exit_cell; cell != start; cell = parent[cell]) {
        path.push_back(previous_move[cell]);
    }
    std::reverse(path.begin(), path.end());

    std::cout << "YES\n" << path.size() << '\n' << path << '\n';
    return 0;
}
