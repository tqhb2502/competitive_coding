#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    const int n = static_cast<int>(s.size());
    const int transformed_length = 2 * n + 1;

    // Biến đổi xâu: chèn ký tự phân cách (giá trị 0) vào giữa mọi cặp ký tự và ở
    // hai đầu, để mọi palindrome đều có độ dài lẻ. Hai lính canh (guard) khác nhau
    // và khác mọi giá trị khác đặt ở hai biên giúp vòng lặp mở rộng tự dừng.
    std::vector<int> transformed(static_cast<std::size_t>(transformed_length + 2), 0);
    transformed[0] = -1;
    transformed[static_cast<std::size_t>(transformed_length + 1)] = -2;
    for (int i = 0; i < n; ++i) {
        transformed[static_cast<std::size_t>(2 * i + 2)] =
            static_cast<unsigned char>(s[static_cast<std::size_t>(i)]);
    }

    std::vector<int> radius(static_cast<std::size_t>(transformed_length + 2), 0);
    int center = 0;
    int right = 0;
    int best_center = 1;
    int best_length = 0;

    // Manacher: quét mọi tâm i, tính bán kính palindrome radius[i].
    for (int i = 1; i <= transformed_length; ++i) {
        // Tận dụng đối xứng qua center để khởi tạo, tránh mở rộng lại từ đầu.
        if (i < right) {
            const int mirror = 2 * center - i;
            radius[static_cast<std::size_t>(i)] =
                std::min(right - i, radius[static_cast<std::size_t>(mirror)]);
        }

        // Mở rộng thêm khi hai bên còn đối xứng (guard bảo đảm dừng ở biên).
        while (transformed[static_cast<std::size_t>(i + radius[static_cast<std::size_t>(i)] + 1)] ==
               transformed[static_cast<std::size_t>(i - radius[static_cast<std::size_t>(i)] - 1)]) {
            ++radius[static_cast<std::size_t>(i)];
        }

        // Cập nhật palindrome vươn xa nhất (con trỏ right không bao giờ lùi).
        if (i + radius[static_cast<std::size_t>(i)] > right) {
            center = i;
            right = i + radius[static_cast<std::size_t>(i)];
        }
        // Lưu tâm cho bán kính lớn nhất (chính là độ dài đáp án trong xâu gốc).
        if (radius[static_cast<std::size_t>(i)] > best_length) {
            best_length = radius[static_cast<std::size_t>(i)];
            best_center = i;
        }
    }

    // Suy ngược vị trí bắt đầu trong xâu gốc rồi in palindrome dài nhất.
    const int start = (best_center - 1 - best_length) / 2;
    std::cout << s.substr(static_cast<std::size_t>(start),
                          static_cast<std::size_t>(best_length))
              << '\n';
    return 0;
}
