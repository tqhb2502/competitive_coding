#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, operations;
    cin >> n >> operations;

    // values[vị trí] = giá trị, position[giá trị] = vị trí
    vector<int> values(n + 1);
    vector<int> position(n + 1);
    for (int index = 1; index <= n; ++index) {
        cin >> values[index];
        position[values[index]] = index;
    }

    // Đếm số cặp "bad" (v, v+1) với position[v] > position[v+1]; đáp án = breaks + 1
    int breaks = 0;
    for (int value = 1; value < n; ++value) {
        if (position[value] > position[value + 1]) {
            ++breaks;
        }
    }

    while (operations--) {
        // Đổi chỗ giá trị ở hai vị trí firstPosition, secondPosition
        int firstPosition, secondPosition;
        cin >> firstPosition >> secondPosition;
        const int firstValue = values[firstPosition];
        const int secondValue = values[secondPosition];

        // Chỉ bốn chỉ số cặp này có thể đổi trạng thái; sort để dễ khử trùng lặp
        array<int, 4> affected = {
            firstValue - 1, firstValue, secondValue - 1, secondValue
        };
        sort(affected.begin(), affected.end());

        // Bỏ đóng góp cũ của các cặp hợp lệ (bỏ qua chỉ số ngoài [1, n-1] và trùng lặp)
        int previous = -1;
        for (const int value : affected) {
            if (value < 1 || value >= n || value == previous) {
                continue;
            }
            breaks -= position[value] > position[value + 1];
            previous = value;
        }

        // Thực hiện swap: cập nhật values và position của hai giá trị
        swap(values[firstPosition], values[secondPosition]);
        position[firstValue] = secondPosition;
        position[secondValue] = firstPosition;

        // Thêm đóng góp mới của đúng những cặp đó sau khi swap
        previous = -1;
        for (const int value : affected) {
            if (value < 1 || value >= n || value == previous) {
                continue;
            }
            breaks += position[value] > position[value + 1];
            previous = value;
        }

        cout << breaks + 1 << '\n';
    }

    return 0;
}
