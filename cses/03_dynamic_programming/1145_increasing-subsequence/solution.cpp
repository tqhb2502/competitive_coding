#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // tails[i] = giá trị đuôi nhỏ nhất có thể của một dãy con tăng nghiêm ngặt
    // độ dài i+1, xét trên tiền tố đã duyệt. Mảng tails luôn tăng nghiêm ngặt.
    std::vector<int> tails;
    tails.reserve(n);

    for (int i = 0; i < n; ++i) {
        int value;
        std::cin >> value;

        // lower_bound: tìm vị trí đầu tiên có giá trị >= value (bảo đảm nghiêm ngặt).
        const auto position =
            std::lower_bound(tails.begin(), tails.end(), value);
        if (position == tails.end()) {
            // value lớn hơn mọi đuôi hiện có -> nối vào cuối, LIS dài thêm 1.
            tails.push_back(value);
        } else {
            // Thay đuôi tốt hơn cho dãy con tăng cùng độ dài.
            *position = value;
        }
    }

    // Số cột của tails chính là độ dài LIS.
    std::cout << tails.size() << '\n';
    return 0;
}
