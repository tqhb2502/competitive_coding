#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using i64 = long long;
using i128 = __int128_t;

// Fenwick tree (BIT): point update cộng dồn, prefix query lấy tổng tiền tố.
class Fenwick {
public:
    explicit Fenwick(int n) : data_(static_cast<std::size_t>(n + 1), 0) {}

    void add(int position, i128 value) {
        const int n = static_cast<int>(data_.size()) - 1;
        for (int i = position; i <= n; i += i & -i) {
            data_[static_cast<std::size_t>(i)] += value;
        }
    }

    [[nodiscard]] i128 prefix_sum(int position) const {
        i128 result = 0;
        for (int i = position; i > 0; i -= i & -i) {
            result += data_[static_cast<std::size_t>(i)];
        }
        return result;
    }

private:
    std::vector<i128> data_;
};

// In số kiểu i128 (std::cout không hỗ trợ trực tiếp kiểu này).
void print_integer(i128 value) {
    if (value == 0) {
        std::cout << '0';
        return;
    }
    if (value < 0) {
        std::cout << '-';
        value = -value;
    }
    std::string digits;
    while (value > 0) {
        digits.push_back(static_cast<char>('0' + static_cast<int>(value % 10)));
        value /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    std::cout << digits;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    // Prefix sum tĩnh của mảng ban đầu (không bao giờ thay đổi).
    std::vector<i64> initial_prefix(static_cast<std::size_t>(n + 1), 0);
    for (int i = 1; i <= n; ++i) {
        i64 value = 0;
        std::cin >> value;
        initial_prefix[static_cast<std::size_t>(i)] =
            initial_prefix[static_cast<std::size_t>(i - 1)] + value;
    }

    // Ba BIT lưu prefix sum của f[j], j*f[j], j^2*f[j] (tức A0, A1, A2).
    Fenwick moment0(n);
    Fenwick moment1(n);
    Fenwick moment2(n);

    // Cộng một điểm sai phân bậc hai vào cả ba BIT; vị trí vượt quá n thì bỏ qua.
    const auto add_second_difference = [&](int position, i128 coefficient) {
        if (position > n) {
            return;
        }
        const i128 p = position;
        moment0.add(position, coefficient);
        moment1.add(position, p * coefficient);
        moment2.add(position, p * p * coefficient);
    };

    // PS(x): prefix sum của phần cộng thêm, tính theo đa thức bậc hai của x.
    const auto added_prefix_sum = [&](int x) -> i128 {
        if (x <= 0) {
            return 0;
        }
        const i128 xx = x;
        const i128 a0 = moment0.prefix_sum(x);
        const i128 a1 = moment1.prefix_sum(x);
        const i128 a2 = moment2.prefix_sum(x);
        return ((xx * xx + 3 * xx + 2) * a0 - (2 * xx + 3) * a1 + a2) / 2;
    };

    while (q-- > 0) {
        int type = 0;
        int left = 0;
        int right = 0;
        std::cin >> type >> left >> right;
        if (type == 1) {
            // Update loại 1: cấp số cộng trên [left, right] rút về ba point update.
            add_second_difference(left, 1);
            add_second_difference(right + 1, -static_cast<i128>(right - left + 2));
            add_second_difference(right + 2, static_cast<i128>(right - left + 1));
        } else {
            // Query loại 2: tổng đoạn = phần ban đầu cộng phần cộng thêm.
            const i128 original =
                initial_prefix[static_cast<std::size_t>(right)] -
                initial_prefix[static_cast<std::size_t>(left - 1)];
            const i128 answer = original + added_prefix_sum(right) - added_prefix_sum(left - 1);
            print_integer(answer);
            std::cout << '\n';
        }
    }
}
