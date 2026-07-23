#include <algorithm>
#include <iostream>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // last_position[id] = chỉ số xuất hiện gần nhất của id đó
    std::unordered_map<int, int> last_position;
    last_position.reserve(static_cast<std::size_t>(n) * 2);

    int left = 0;  // biên trái của cửa sổ trượt hiện tại
    int best = 0;

    // Duyệt con trỏ phải; mở rộng cửa sổ [left, right] gồm các id khác nhau
    for (int right = 0; right < n; ++right) {
        int song;
        std::cin >> song;

        // Nếu id lặp lại và bản cũ còn nằm trong cửa sổ thì nhảy biên trái
        const auto it = last_position.find(song);
        if (it != last_position.end() && it->second >= left) {
            left = it->second + 1;
        }
        last_position[song] = right;

        // Cập nhật độ dài lớn nhất của dãy hợp lệ kết thúc tại right
        best = std::max(best, right - left + 1);
    }

    std::cout << best << '\n';
    return 0;
}
