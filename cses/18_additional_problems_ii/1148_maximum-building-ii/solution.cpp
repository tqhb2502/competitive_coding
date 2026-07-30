#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows = 0;
    int columns = 0;
    cin >> rows >> columns;

    // Hai mảng hiệu để cộng dồn các đoạn tuyến tính vào bảng exact[H][w]:
    // slopeDifference giữ hệ số của w, constantDifference giữ hằng số.
    const int stride = columns + 2;
    const size_t tableSize = static_cast<size_t>(rows + 2) *
                             static_cast<size_t>(stride);
    vector<long long> slopeDifference(tableSize, 0);
    vector<long long> constantDifference(tableSize, 0);

    const auto position = [stride](int height, int width) {
        return static_cast<size_t>(height) * static_cast<size_t>(stride) +
               static_cast<size_t>(width);
    };
    // Cộng một đoạn tuyến tính (slope*w + constant) cho mọi độ rộng w trong
    // khoảng [leftWidth, rightWidth] bằng kỹ thuật mảng hiệu theo cột w.
    const auto addLinearRange = [&](int height, int leftWidth, int rightWidth,
                                    long long slope, long long constant) {
        if (leftWidth > rightWidth) {
            return;
        }
        slopeDifference[position(height, leftWidth)] += slope;
        slopeDifference[position(height, rightWidth + 1)] -= slope;
        constantDifference[position(height, leftWidth)] += constant;
        constantDifference[position(height, rightWidth + 1)] -= constant;
    };

    vector<int> histogram(columns, 0);         // Chiều cao ô trống liên tiếp đi lên
    vector<int> previousSmaller(columns, -1);  // Phần tử nhỏ hơn gần nhất bên trái
    vector<int> nextSmallerOrEqual(columns, columns);  // Nhỏ hơn hoặc bằng bên phải
    vector<int> stack;
    stack.reserve(columns);

    for (int row = 0; row < rows; ++row) {
        // Cập nhật histogram theo hàng dưới hiện tại: gặp cây thì reset về 0.
        string line;
        cin >> line;
        for (int column = 0; column < columns; ++column) {
            if (line[column] == '.') {
                ++histogram[column];
            } else {
                histogram[column] = 0;
            }
        }

        // Stack đơn điệu: tìm phần tử nhỏ hơn gần nhất bên trái mỗi cột.
        stack.clear();
        for (int column = 0; column < columns; ++column) {
            while (!stack.empty() &&
                   histogram[stack.back()] >= histogram[column]) {
                stack.pop_back();
            }
            previousSmaller[column] = stack.empty() ? -1 : stack.back();
            stack.push_back(column);
        }

        // Stack đơn điệu: tìm phần tử nhỏ hơn hoặc bằng gần nhất bên phải mỗi cột.
        stack.clear();
        for (int column = columns - 1; column >= 0; --column) {
            while (!stack.empty() &&
                   histogram[stack.back()] > histogram[column]) {
                stack.pop_back();
            }
            nextSmallerOrEqual[column] =
                stack.empty() ? columns : stack.back();
            stack.push_back(column);
        }

        for (int column = 0; column < columns; ++column) {
            // Mỗi cột là cực tiểu quản lý các đoạn có giá trị nhỏ nhất là height.
            const int height = histogram[column];
            if (height == 0) {
                continue;
            }
            // Số cách mở rộng đầu trái và đầu phải quanh cột cực tiểu này.
            const int leftChoices = column - previousSmaller[column];
            const int rightChoices = nextSmallerOrEqual[column] - column;
            const int smallerSide = min(leftChoices, rightChoices);
            const int largerSide = max(leftChoices, rightChoices);
            const int maximumWidth = leftChoices + rightChoices - 1;

            // Số cặp (a, b) theo từng độ rộng w là hàm tam giác gồm ba đoạn tuyến tính.
            addLinearRange(height, 1, smallerSide, 1, 0);
            addLinearRange(height, smallerSide + 1, largerSide, 0,
                           smallerSide);
            addLinearRange(height, largerSide + 1, maximumWidth, -1,
                           leftChoices + rightChoices);
        }
    }

    // Lấy prefix theo w để khôi phục exact[H][w] = số đoạn có minimum đúng bằng H.
    for (int height = 1; height <= rows; ++height) {
        long long slope = 0;
        long long constant = 0;
        for (int width = 1; width <= columns; ++width) {
            slope += slopeDifference[position(height, width)];
            constant += constantDifference[position(height, width)];
            slopeDifference[position(height, width)] =
                slope * width + constant;
        }
    }

    // Lấy suffix theo H: tổng exact[H][w] với H >= h chính là số hình h*w.
    for (int height = rows; height >= 1; --height) {
        for (int width = 1; width <= columns; ++width) {
            slopeDifference[position(height, width)] +=
                slopeDifference[position(height + 1, width)];
        }
    }

    // In bảng kết quả kích thước rows*columns.
    for (int height = 1; height <= rows; ++height) {
        for (int width = 1; width <= columns; ++width) {
            cout << slopeDifference[position(height, width)]
                 << (width == columns ? '\n' : ' ');
        }
    }
}
