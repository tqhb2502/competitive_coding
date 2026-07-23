#include <array>
#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc bàn cờ 8x8: mỗi ô là '.' (tự do) hoặc '*' (bị cấm).
    array<string, 8> board{};
    for (string& row : board) {
        cin >> row;
    }

    // Mảng đánh dấu trạng thái đã bị chiếm.
    array<bool, 8> column{};         // cột j đã có quân hậu chưa
    array<bool, 15> mainDiagonal{};  // đường chéo chính, chỉ số (row - col + 7)
    array<bool, 15> antiDiagonal{};  // đường chéo phụ, chỉ số (row + col)
    int answer = 0;

    // Quay lui theo từng hàng, mỗi hàng đặt đúng một quân hậu.
    auto backtrack = [&](auto&& self, int row) -> void {
        // Đặt xong cả 8 hàng => tìm được một cách hợp lệ.
        if (row == 8) {
            ++answer;
            return;
        }

        // Thử đặt quân hậu vào từng cột của hàng hiện tại.
        for (int col = 0; col < 8; ++col) {
            const int diagonal1 = row - col + 7;
            const int diagonal2 = row + col;
            // Bỏ qua nếu ô bị cấm hoặc cột / đường chéo đã bị chiếm.
            if (board[row][col] == '*' || column[col] ||
                mainDiagonal[diagonal1] || antiDiagonal[diagonal2]) {
                continue;
            }

            // Đánh dấu trạng thái rồi đệ quy sang hàng tiếp theo.
            column[col] = true;
            mainDiagonal[diagonal1] = true;
            antiDiagonal[diagonal2] = true;

            self(self, row + 1);

            // Bỏ đánh dấu (backtrack) để thử cột khác.
            column[col] = false;
            mainDiagonal[diagonal1] = false;
            antiDiagonal[diagonal2] = false;
        }
    };

    backtrack(backtrack, 0);
    cout << answer << '\n';
    return 0;
}
