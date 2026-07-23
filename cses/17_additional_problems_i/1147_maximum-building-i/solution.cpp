#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows, columns;
    cin >> rows >> columns;
    // height[c]: số ô trống liên tiếp theo chiều dọc kết thúc tại hàng hiện tại
    vector<int> height(columns + 1, 0);
    int answer = 0;

    for (int row = 0; row < rows; ++row) {
        string line;
        cin >> line;
        // Cập nhật histogram: gặp '.' thì tăng chiều cao, gặp '*' thì đặt lại 0
        for (int column = 0; column < columns; ++column) {
            if (line[column] == '.') {
                ++height[column];
            } else {
                height[column] = 0;
            }
        }

        // Tìm hình chữ nhật lớn nhất trong histogram bằng monotonic stack.
        // Cột thứ columns đóng vai trò cột biên (sentinel) cao 0 để pop hết stack.
        vector<int> increasingStack;
        increasingStack.reserve(columns + 1);
        for (int column = 0; column <= columns; ++column) {
            // Khi cột hiện tại thấp hơn, pop các thanh cao hơn và tính diện tích
            while (!increasingStack.empty() &&
                   height[increasingStack.back()] > height[column]) {
                const int currentHeight = height[increasingStack.back()];
                increasingStack.pop_back();
                // Biên trái là ngay sau chỉ số còn lại trên stack
                const int left = increasingStack.empty() ? 0 : increasingStack.back() + 1;
                answer = max(answer, currentHeight * (column - left));
            }
            increasingStack.push_back(column);
        }
    }

    cout << answer << '\n';
}
