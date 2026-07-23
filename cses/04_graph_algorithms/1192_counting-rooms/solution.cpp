#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Đệm một viền tường quanh lưới: mảng phẳng (n+2) x (m+2),
    // mặc định toàn 0 (tường) để khỏi kiểm tra biên khi mở rộng lân cận.
    const int width = m + 2;
    std::vector<char> floor((n + 2) * width, false);
    for (int row = 1; row <= n; ++row) {
        std::string line;
        std::cin >> line;
        for (int column = 1; column <= m; ++column) {
            // '.' -> sàn (true = 1), '#' -> tường (false = 0)
            floor[row * width + column] = (line[column - 1] == '.');
        }
    }

    int rooms = 0;
    std::vector<int> stack;
    stack.reserve(n * m);

    // Duyệt toàn lưới: mỗi ô sàn chưa thăm mở đầu một phòng mới.
    for (int start = 0; start < static_cast<int>(floor.size()); ++start) {
        if (!floor[start]) {
            continue;
        }

        ++rooms;
        floor[start] = false;  // đánh dấu đã thăm bằng cách đặt lại thành tường
        stack.push_back(start);

        // Flood fill (DFS lặp) quét trọn thành phần liên thông hiện tại.
        while (!stack.empty()) {
            const int cell = stack.back();
            stack.pop_back();

            // 4 lân cận: trái, phải, trên, dưới (nhờ viền, không cần kiểm biên).
            const int neighbors[4] = {cell - 1, cell + 1, cell - width,
                                      cell + width};
            for (int neighbor : neighbors) {
                if (floor[neighbor]) {
                    floor[neighbor] = false;
                    stack.push_back(neighbor);
                }
            }
        }
    }

    std::cout << rooms << '\n';
    return 0;
}
