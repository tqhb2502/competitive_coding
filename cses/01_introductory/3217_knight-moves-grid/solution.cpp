#include <array>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Trải bàn cờ thành mảng 1 chiều phẳng có viền dày 2 ô mỗi bên.
    // Viền = -2 (tường, coi như đã thăm); vùng n x n bên trong = -1 (chưa thăm).
    const int width = n + 4;
    vector<int> distance(width * width, -2);
    for (int row = 0; row < n; ++row) {
        const int first = (row + 2) * width + 2;
        for (int col = 0; col < n; ++col) {
            distance[first + col] = -1;
        }
    }

    // Góc (0,0) nằm ở tọa độ có viền (2,2) -> chỉ số phẳng start.
    const int start = 2 * width + 2;
    distance[start] = 0;

    // 8 nước đi của quân mã đổi thành 8 độ lệch chỉ số trên mảng phẳng.
    const array<int, 8> offsets = {
        width + 2, width - 2, -width + 2, -width - 2,
        2 * width + 1, 2 * width - 1, -2 * width + 1, -2 * width - 1
    };

    // BFS bằng hàng đợi thủ công (mảng + con trỏ head), xuất phát từ góc (0,0).
    // Viền -2 tự động chặn nước đi ra ngoài nên không cần kiểm tra biên.
    vector<int> queue;
    queue.reserve(n * n);
    queue.push_back(start);
    for (size_t head = 0; head < queue.size(); ++head) {
        const int current = queue[head];
        for (const int offset : offsets) {
            const int next = current + offset;
            if (distance[next] == -1) {          // chỉ đi tới ô chưa thăm
                distance[next] = distance[current] + 1;
                queue.push_back(next);
            }
        }
    }

    // In ma trận khoảng cách, chỉ lấy phần bên trong, các số cách nhau bởi dấu cách.
    for (int row = 0; row < n; ++row) {
        const int first = (row + 2) * width + 2;
        for (int col = 0; col < n; ++col) {
            if (col > 0) {
                cout << ' ';
            }
            cout << distance[first + col];
        }
        cout << '\n';
    }

    return 0;
}
