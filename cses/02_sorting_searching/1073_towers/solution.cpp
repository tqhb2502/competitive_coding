#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // tops: đỉnh hiện tại của mọi tháp, luôn giữ sắp xếp tăng dần (patience sorting)
    std::vector<int> tops;
    tops.reserve(n);

    for (int i = 0; i < n; ++i) {
        int cube;
        std::cin >> cube;

        // Tìm tháp có đỉnh nhỏ nhất nhưng lớn hơn THỰC SỰ cube (upper_bound)
        const auto it = std::upper_bound(tops.begin(), tops.end(), cube);
        if (it == tops.end()) {
            // Không có tháp phù hợp -> tạo tháp mới, thêm vào cuối vẫn giữ thứ tự
            tops.push_back(cube);
        } else {
            // Đặt cube lên tháp đó; gán tại chỗ vẫn giữ tops tăng dần
            *it = cube;
        }
    }

    // Đáp số: số tháp tối thiểu chính là số phần tử của tops
    std::cout << tops.size() << '\n';
    return 0;
}
