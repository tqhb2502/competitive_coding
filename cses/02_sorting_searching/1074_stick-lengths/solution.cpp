#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Đọc độ dài các que; dùng long long vì tổng chi phí có thể lớn.
    std::vector<long long> lengths(n);
    for (long long& length : lengths) {
        std::cin >> length;
    }

    // Sắp xếp rồi lấy median: điểm làm tổng khoảng cách đạt cực tiểu.
    std::sort(lengths.begin(), lengths.end());
    const long long median = lengths[n / 2];

    // Cộng dồn |p_i - median| để ra tổng chi phí nhỏ nhất.
    long long answer = 0;
    for (const long long length : lengths) {
        answer += (length >= median ? length - median : median - length);
    }

    std::cout << answer << '\n';
    return 0;
}
