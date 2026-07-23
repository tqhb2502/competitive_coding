#include <algorithm>
#include <iostream>

using namespace std;

using Integer = long long;

// Số nước mã ít nhất để đi từ khoảng lệch (first, second) về gốc (0, 0).
Integer knightDistance(Integer first, Integer second) {
    // Hoán đổi để bảo đảm first >= second.
    if (first < second) {
        swap(first, second);
    }
    // Ba cấu hình nhỏ có đáp án đặc biệt, không theo công thức tổng quát.
    if (first == 1 && second == 0) {
        return 3;
    }
    if (first == 1 && second == 1) {
        return 4;
    }
    if (first == 2 && second == 2) {
        return 4;
    }
    // Cận dưới: từ giới hạn theo một tọa độ và theo tổng Manhattan.
    Integer moves = max((first + 1) / 2, (first + second + 2) / 3);
    // Chỉnh tính chẵn lẻ cho khớp với tổng tọa độ.
    if ((moves + first + second) % 2 != 0) {
        ++moves;
    }
    return moves;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int queries;
    cin >> queries;
    while (queries-- > 0) {
        Integer row;
        Integer column;
        cin >> row >> column;
        // Dời gốc tọa độ về ô (1, 1) trước khi tính khoảng cách.
        cout << knightDistance(row - 1, column - 1) << '\n';
    }
}
