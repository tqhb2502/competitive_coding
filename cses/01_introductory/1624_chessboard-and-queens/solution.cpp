#include <array>
#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<string, 8> board{};
    for (string& row : board) {
        cin >> row;
    }

    array<bool, 8> column{};
    array<bool, 15> mainDiagonal{};
    array<bool, 15> antiDiagonal{};
    int answer = 0;

    auto backtrack = [&](auto&& self, int row) -> void {
        if (row == 8) {
            ++answer;
            return;
        }

        for (int col = 0; col < 8; ++col) {
            const int diagonal1 = row - col + 7;
            const int diagonal2 = row + col;
            if (board[row][col] == '*' || column[col] ||
                mainDiagonal[diagonal1] || antiDiagonal[diagonal2]) {
                continue;
            }

            column[col] = true;
            mainDiagonal[diagonal1] = true;
            antiDiagonal[diagonal2] = true;

            self(self, row + 1);

            column[col] = false;
            mainDiagonal[diagonal1] = false;
            antiDiagonal[diagonal2] = false;
        }
    };

    backtrack(backtrack, 0);
    cout << answer << '\n';
    return 0;
}
