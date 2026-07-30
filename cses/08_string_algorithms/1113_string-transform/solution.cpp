#include <array>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc L = cột cuối (last column) của ma trận rotation đã sắp xếp.
    std::string last_column;
    std::cin >> last_column;
    const int length = static_cast<int>(last_column.size());

    // Đếm số lần xuất hiện của từng ký tự trong L.
    std::array<int, 256> frequency{};
    for (const unsigned char character : last_column) {
        ++frequency[character];
    }

    // first_position[c] = C[c] = số ký tự nhỏ hơn c, tức vị trí bắt đầu của c
    // trong cột đầu F (thứ tự byte trùng thứ tự từ điển vì '#'(35) < 'a'(97)).
    std::array<int, 256> first_position{};
    int total = 0;
    for (int character = 0; character < 256; ++character) {
        first_position[static_cast<std::size_t>(character)] = total;
        total += frequency[static_cast<std::size_t>(character)];
    }

    // Tính LF mapping: LF[row] = C[L[row]] + rank, với rank là thứ tự xuất hiện
    // của L[row] trong L. next_position giữ con trỏ ghi hiện tại cho mỗi ký tự.
    std::array<int, 256> next_position = first_position;
    std::vector<int> lf(static_cast<std::size_t>(length));
    for (int row = 0; row < length; ++row) {
        const unsigned char character =
            static_cast<unsigned char>(last_column[static_cast<std::size_t>(row)]);
        lf[static_cast<std::size_t>(row)] = next_position[character]++;
    }

    // Đi theo LF, ghi ngược từ cuối về đầu để dựng lại T = '#' + s.
    std::string restored(static_cast<std::size_t>(length), ' ');
    int row = 0;  // Hàng đã sắp xếp đầu tiên luôn bắt đầu bằng sentinel '#' duy nhất.
    for (int output_index = length - 1; output_index >= 0; --output_index) {
        restored[static_cast<std::size_t>(output_index)] =
            last_column[static_cast<std::size_t>(row)];
        row = lf[static_cast<std::size_t>(row)];
    }

    // restored = '#' + chuỗi gốc; bỏ ký tự '#' ở đầu để in ra s.
    std::cout << restored.substr(1) << '\n';
    return 0;
}
