#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string sequence;
    std::cin >> sequence;

    int best = 0;        // độ dài đoạn lặp dài nhất tìm được
    int current = 0;     // độ dài dãy ký tự giống nhau liên tiếp hiện tại
    char previous = '\0';

    // Duyệt chuỗi một lần từ trái sang phải.
    for (char nucleotide : sequence) {
        if (nucleotide == previous) {
            ++current;              // vẫn cùng ký tự -> kéo dài đoạn hiện tại
        } else {
            previous = nucleotide;  // đổi ký tự -> bắt đầu đoạn mới
            current = 1;
        }
        best = std::max(best, current);  // cập nhật đáp án sau mỗi bước
    }

    std::cout << best << '\n';
    return 0;
}
