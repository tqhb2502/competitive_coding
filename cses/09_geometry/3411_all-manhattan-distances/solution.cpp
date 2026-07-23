#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Tổng có thể lớn cỡ 10^20 (vượt 64-bit) nên dùng __int128 để tránh tràn số.
using WideInteger = __int128_t;

// Bài toán một chiều: tính tổng |a_i - a_j| trên mọi cặp bằng sort + prefix sum.
WideInteger sum_pairwise_distances(std::vector<std::int64_t>& values) {
    // Sắp xếp tăng dần để mọi phần tử trước index đều nhỏ hơn hoặc bằng values[index].
    std::sort(values.begin(), values.end());
    WideInteger result = 0;
    WideInteger prefix_sum = 0;  // Tổng tiền tố a[0..index-1].
    for (std::size_t index = 0; index < values.size(); ++index) {
        const WideInteger value = values[index];
        // Đóng góp của phần tử hiện tại khi ghép với các phần tử đứng trước:
        //   value * index - (a[0] + ... + a[index-1]).
        result += value * static_cast<WideInteger>(index) - prefix_sum;
        prefix_sum += value;
    }
    return result;
}

// In số __int128 (std::cout không hỗ trợ trực tiếp) bằng cách tách từng chữ số.
void print_wide_integer(WideInteger value) {
    if (value == 0) {
        std::cout << "0\n";
        return;
    }

    std::string digits;
    while (value > 0) {
        digits.push_back(static_cast<char>('0' + value % 10));
        value /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    std::cout << digits << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int point_count = 0;
    std::cin >> point_count;
    std::vector<std::int64_t> xs(static_cast<std::size_t>(point_count));
    std::vector<std::int64_t> ys(static_cast<std::size_t>(point_count));
    for (int i = 0; i < point_count; ++i) {
        std::cin >> xs[static_cast<std::size_t>(i)] >> ys[static_cast<std::size_t>(i)];
    }

    // Tách độc lập theo chiều x và chiều y rồi cộng lại thành đáp án cuối cùng.
    print_wide_integer(sum_pairwise_distances(xs) + sum_pairwise_distances(ys));
    return 0;
}
