#include <cstdint>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc toạ độ ô cần truy vấn (chỉ số bắt đầu từ 1)
    int64_t row = 0;
    int64_t column = 0;
    cin >> row >> column;

    // Đổi sang chỉ số từ 0 rồi lấy XOR: giá trị ô đúng bằng (y-1) XOR (x-1)
    cout << ((row - 1) ^ (column - 1)) << '\n';
}
