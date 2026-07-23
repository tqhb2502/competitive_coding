#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    if (!(std::cin >> text)) {
        return 0;
    }

    const int length = static_cast<int>(text.size());

    // Z-function: duy trì cửa sổ [left, right) là đoạn khớp prefix vươn xa nhất
    std::vector<int> z_function(length, 0);
    int left = 0;
    int right = 0;
    for (int index = 1; index < length; ++index) {
        // Nếu còn nằm trong cửa sổ thì tận dụng giá trị đã tính trước
        if (index < right) {
            z_function[index] = std::min(right - index, z_function[index - left]);
        }
        // Mở rộng bằng cách so sánh trực tiếp với prefix
        while (index + z_function[index] < length &&
               text[z_function[index]] == text[index + z_function[index]]) {
            ++z_function[index];
        }
        // Cập nhật cửa sổ khi khớp được xa hơn về bên phải
        if (index + z_function[index] > right) {
            left = index;
            right = index + z_function[index];
        }
    }

    // Prefix function (KMP): border dài nhất vừa là prefix vừa là suffix
    std::vector<int> prefix_function(length, 0);
    for (int index = 1; index < length; ++index) {
        int border = prefix_function[index - 1];
        // Quay lui theo chuỗi border khi ký tự không khớp
        while (border > 0 && text[index] != text[border]) {
            border = prefix_function[border - 1];
        }
        if (text[index] == text[border]) {
            ++border;
        }
        prefix_function[index] = border;
    }

    // Dòng 1: in Z-function, dòng 2: in prefix function
    for (int index = 0; index < length; ++index) {
        if (index > 0) {
            std::cout << ' ';
        }
        std::cout << z_function[index];
    }
    std::cout << '\n';
    for (int index = 0; index < length; ++index) {
        if (index > 0) {
            std::cout << ' ';
        }
        std::cout << prefix_function[index];
    }
    std::cout << '\n';

    return 0;
}
