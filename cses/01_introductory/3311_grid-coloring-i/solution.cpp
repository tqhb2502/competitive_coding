#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc kích thước lưới và lưới ký tự gốc
    int n, m;
    cin >> n >> m;
    vector<string> original(n);
    for (string& row : original) {
        cin >> row;
    }

    const array<char, 4> colors = {'A', 'B', 'C', 'D'};
    vector<string> answer(n, string(m, '?'));

    // Duyệt row-major, tô tham lam từng ô
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < m; ++col) {
            // Chọn màu đầu tiên khác màu gốc, khác ô trên và khác ô trái
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

    // Luôn tồn tại lời giải nên chỉ cần in lưới kết quả
    for (const string& row : answer) {
        cout << row << '\n';
    }

    return 0;
}
