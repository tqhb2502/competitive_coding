#include <cmath>
#include <iostream>
#include <tuple>

// Căn bậc hai nguyên: làm tròn từ sqrt double rồi hiệu chỉnh để tránh sai số.
int integer_square_root(const int value) {
    int root = static_cast<int>(std::sqrt(static_cast<double>(value)));
    while ((root + 1) * (root + 1) <= value) {
        ++root;
    }
    while (root * root > value) {
        --root;
    }
    return root;
}

// Kiểm tra value có là tổng 2 số chính phương không bằng kỹ thuật two-pointer:
// tăng low nếu tổng nhỏ hơn value, giảm high nếu lớn hơn. O(sqrt(value)).
bool two_squares(const int value, int& first, int& second) {
    int low = 0;
    int high = integer_square_root(value);
    while (low <= high) {
        const int sum = low * low + high * high;
        if (sum == value) {
            first = low;
            second = high;
            return true;
        }
        if (sum < value) {
            ++low;
        } else {
            --high;
        }
    }
    return false;
}

// Tìm 3 số chính phương có tổng bằng value (giả thiết value không thuộc dạng
// cấm của Legendre). Duyệt third giảm dần, kiểm tra phần dư có tách được thành
// 2 số chính phương không.
std::tuple<int, int, int> three_squares(const int value) {
    for (int third = integer_square_root(value); third >= 0; --third) {
        int first = 0;
        int second = 0;
        if (two_squares(value - third * third, first, second)) {
            return {first, second, third};
        }
    }
    return {0, 0, 0};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        int value;
        std::cin >> value;
        if (value == 0) {
            std::cout << "0 0 0 0\n";
            continue;
        }

        // Bước 1 - khử hết thừa số 4: value = 4^k * reduced, scale = 2^k.
        // Nghiệm của reduced nhân lại với scale cho nghiệm của value.
        int reduced = value;
        int scale = 1;
        while (reduced % 4 == 0) {
            reduced /= 4;
            scale *= 2;
        }

        // Bước 2 - xử lý dạng cấm: nếu reduced ≡ 7 (mod 8) thì tách riêng
        // fourth = 1, phần còn lại reduced-1 chắc chắn là tổng 3 số chính phương.
        int fourth = 0;
        int target = reduced;
        if (reduced % 8 == 7) {
            target = reduced - 1;
            fourth = 1;
        }
        // Bước 3 - giải target bằng three_squares rồi nhân lại với scale.
        const auto [first, second, third] = three_squares(target);
        std::cout << first * scale << ' ' << second * scale << ' '
                  << third * scale << ' ' << fourth * scale << '\n';
    }
    return 0;
}
